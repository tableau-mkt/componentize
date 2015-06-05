<?php
/**
 * @file Web component object for Drupal, uses Handlebars and SASS.
 */

namespace Component;
use LightnCandy;

class Component {

  private $name,
          $configs,
          $styleguide,
          $namespace,
          $modifiers,
          $modifier,
          $variables,
          $template,
          $template_dir;

  /**
   * Constructor.
   */
  public function __construct($name, $configs, $styleguide) {
    $this->name = $name;
    $this->configs = $configs;
    $this->styleguide = $styleguide;

    // Common.
    $this->template_dir = variable_get('components_templates', COMPONENTS_COMPILED_TEMPLATES);

    // Full storage namespace.
    $this->namespace = $this->configs['module'] . '-' .  $this->name;

    // Build out component.
    $data = $this->load();
    $this->template = $data['template'];
    $this->variables = $data['variables'];
    $this->modifiers = $data['modifiers'];
  }


  /**
   * Process data via template.
   *
   * @param array $data
   *   Key value pairs for template variables.
   *
   * @return string
   */
  public function render($data) {
    // Double check folder.
    if (!file_prepare_directory($this->template_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      drupal_set_message(t(
        'Unable to create Components template cache directory. Check the permissions on your files directory.'
      ), 'error');
      return;
    }

    // Limited scope assets.
    $path = drupal_get_path('module', $this->configs['module']) . '/';
    if (isset($this->configs['css'])) {
      drupal_add_css($path . $this->configs['css']);
    }
    if (isset($this->configs['js'])) {
      drupal_add_css($path . $this->configs['js']);
    }



    // Stash compiled (PHP) version of template.
    $filepath = $this->template_dir . '/' . $this->namespace . '.php';
    if (!file_exists($filepath) || $this->configs['storage'] === 'none' || $this->configs['reset']) {
      $handlebar = new LightnCandy();
      $compiled = $handlebar->compile($this->template);
      file_unmanaged_save_data($compiled, $filepath, FILE_EXISTS_REPLACE);
    }
    $renderer = include($filepath);

    // Allow external access.
    drupal_alter('components_render', $this, $data);

    // Add inherant details.
    $data = array_merge($data, array(
      'modifier_class' => preg_replace('/^(\.|#)/', '', $this->modifier)
    ));

    return $renderer($data);
  }


  /**
   * Retrieve name of component.
   *
   * @return array
   */
  public function getName() {
    return $this->name;
  }


  /**
   * Retrieve friendly name of component.
   *
   * @return array
   */
  public function getTitle() {
    $section = $this->styleguide->getSection($this->name);
    return $section->getTitle();
  }


  /**
   * Retrieve list of modifiers.
   *
   * @return array
   */
  public function getModifiers() {
    return $this->modifiers;
  }


  /**
   * Choose modifier state for later rendering, strip CSS selector prefix.
   *
   * @param string $modifier
   */
  public function setModifier($modifier) {
    $this->modifier = $modifier;
  }


  /**
   * Discover modifier state.
   *
   * @return string $modifier
   */
  public function getModifier() {
    return $this->modifier;
  }


  /**
   * Retrieve list of variables.
   *
   * @return array
   */
  public function getVariables() {
    return $this->variables;
  }


  /**
   * Choose modifier state for later rendering, strip CSS selector prefix.
   *
   * @param string $modifier
   */
  public function getSection() {
    // @todo Probably a better way within library.
    return current(explode('.', $this->name));
  }


  /**
   * Remove all stored records.
   *
   * @todo Delete entities.
   */
  public function remove() {
    // Storage.
    cache_clear_all($this->namespace, 'cache');
    variable_del($this->namespace);
    // Compiled template.
    file_unmanaged_delete($this->template_dir . '/' . $this->namespace . '.php');
  }


  /**
   * Provide data about this component.
   *
   * @param string $path
   *   Where to find the template.
   *
   * @return array
   */
  private function load() {
    $component_data = &drupal_static(__FUNCTION__ . $this->namespace);

    // User static, or stored output.
    if ($component_data || $component_data = $this->retrieve()) {
      return $component_data;
    }

    $section = $this->styleguide->getSection($this->name);
    $shortName = $section->getTitle();

    // Variable names within template (assignment test).
    $variables = array();
    if ($data = $this->openComponent($shortName . '/' . $shortName . '.json')) {
      $variables = array_keys(json_decode($data, TRUE)) ?: array();
    }

    // Modifiers.
    $modifiers = array();
    $section = $this->styleguide->getSection($this->name);
    foreach ($section->getModifiers() as $modifier) {
      $modifiers[] = $modifier->getName();
    }

    // Classes.
    //$classes = $section->getClassName();

    // Template.
    //$section->getMarkup();
    $template = $this->openComponent($shortName . '/' . $shortName . '.hbs');

    // Javascript dependency.
    // $js_filepath = $this->configs['path'] . '/' . $this->name . '/' . $this->name . '.js';
    // if (file_exists($js_filepath)) {
    //   $js = $js_filepath;
    // }

    // Prepage for storage and retrevial.
    $component_data = array(
      'template' => $template ?: '',
      //'renderer' => $handlebar->compile($data) ?: FALSE,
      'variables' => $variables,
      'modifiers' => $modifiers,
      //'classes' => $classes,
      //'js' => $js,
    );
    $this->save($component_data);

    return $component_data;
  }


  /**
   * File handler utility.
   *
   * @param $filename
   *
   * @return sting
   *   File contents with line breaks;
   */
  private function openComponent($filename) {
    $filepath = $this->configs['path'] . '/'. $filename;
    if (file_exists($filepath)) {
      return file_get_contents($filepath);
    }
    else {
      drupal_set_message(t(
        'Web component file missing: @file', array('@file' => $filepath)
      ), 'warning');
      return FALSE;
    }
  }


  /**
   * Set: Allow alternate config storage options.
   *
   * @todo Save as entities.
   *
   * @param mixed $data
   */
  private function save($data) {
    switch ($this->configs['storage']) {
      case 'variable':
        variable_set('components_' . $this->namespace, $data);
        break;

      case 'cache':
        cache_set('components_' . $this->namespace, $data);
        break;
    }
  }


  /**
   * Get: Allow alternate config caching options.
   *
   * @todo Retrieve as entities.
   */
  private function retrieve() {
    if ($this->configs['reset']) {
      return array();
    }

    switch ($this->configs['storage']) {
      case 'variable':
        return variable_get('components_' . $this->namespace);
        break;

      case 'cache':
        return cache_get('components_' . $this->namespace, 'cache');
        break;

      default:
        return array();
        break;
    }
  }
}
