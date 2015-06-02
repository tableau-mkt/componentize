<?php
/**
 * @file Web component object for Drupal, uses Handlebars and SASS.
 */

namespace Component;

use LightnCandy;


class Component {

  public  $name;

  private $configs,
          $styleguide,
          $namespace,
          $modifiers,
          $modifier,
          $template,
          $variables;

  /**
   * Constructor.
   */
  public function __construct($name, $configs, $styleguide) {
    $this->name = $name;
    $this->configs = $configs;
    $this->styleguide = $styleguide;

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

    //$path = 'public://components';
    $template_dir = variable_get('components_templates', COMPONENTS_COMPILED_TEMPLATES);

    // Double check folder.
    if (!file_prepare_directory($template_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      drupal_set_message(t(
        'Unable to create Components template cache directory. Check the permissions on your files directory.'
      ), 'error');
      return;
    }

    // Add inherant details.
    $data = array_merge($data, array(
      'modifier_class' => $this->modifier
    ));

    // Stash compiled (PHP) version of template.
    // @todo Allow for private files.

    $filepath = $template_dir . '/' . $this->namespace . '.php';
    if (!file_exists($filepath)) {
      $handlebar = new LightnCandy();
      $compiled = $handlebar->compile($this->template);
      file_unmanaged_save_data($compiled, $filepath, FILE_EXISTS_REPLACE);
    }
    $renderer = include($filepath);
    return $renderer($data);
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
   * Choose modifier state for later rendering.
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
  public function getModifier($modifier) {
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
  * Set: Remove all stored records.
  *
  * @todo Delete entities.
  */
  public function delete() {
   variable_del($this->namespace, $data);
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
    $shortName = end(explode('.', $this->name));

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
    try {
      return file_get_contents($filepath);
    }
    catch(Exception $e) {
      drupal_set_message('components', 'Web component file missing: @file',
          array('@file' => $filepath), WATCHDOG_ERROR);
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
      case 2:
        variable_set($this->namespace, $data);
        break;

      case 1:
        cache_set($this->namespace, $data);
        break;
    }
  }


  /**
   * Get: Allow alternate config caching options.
   *
   * @todo Retrieve as entities.
   */
  private function retrieve() {
    switch ($this->configs['storage']) {
      case 2:
        variable_get($this->namespace);
        break;

      case 1:
        cache_get($this->namespace);
        break;

      default:
        return array();
        break;
    }
  }
}
