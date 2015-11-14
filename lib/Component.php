<?php
/**
 * @file Component object for Drupal use, uses Handlebars and SASS.
 */

namespace Componentize;
use LightnCandy;

class Component {

  private $name,
          $configs,
          $styleguide,
          $namespace,
          $section,
          $title,
          $modifiers,
          $modifier,
          $variables,
          $template,
          $template_dir;

  /**
   * Constructor.
   */
  public function __construct($name, $configs = array()) {
    $this->name = $name;
    $this->configs = $configs;
    $this->namespace = $this->configs['module'] . '-' .  $this->name;

    // Common.
    $this->template_dir = variable_get('componentize_templates', COMPONENTIZE_COMPILED_TEMPLATES);

    // Fresh load requested, factory will handle loading with style guide Parser.
    if ($this->configs['reset'] || $this->configs['storage'] === 'none') {
      $this->load();
    }
    else {
      // Attempt to load from storage.
      $this->retrieve();
    }
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
        'Unable to create component template cache directory. Check the permissions on your files directory.'
      ), 'error');
      return;
    }

    // Limited scope assets.
    $path = drupal_get_path('module', $this->configs['module']) . '/';
    if (isset($this->configs['css'])) {
      drupal_add_css($path . $this->configs['css']);
    }
    if (isset($this->configs['js'])) {
      drupal_add_js($path . $this->configs['js']);
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
    drupal_alter('componentize_render', $this, $data);

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
    return $this->title;
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
   * Provide access to template.
   *
   * @return string
   */
  public function getTemplate() {
    return $this->template;
  }


  /**
   * Choose modifier state for later rendering, strip CSS selector prefix.
   *
   * @todo Store as property, load() via $section->thisgetTitle(). #7
   *
   * @param string $modifier
   */
  public function getSection() {
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
   * Gather data about this component from the source.
   *
   * @param string $path
   *   Where to find the template.
   *
   * @return array
   */
  public function load($styleguide = FALSE) {
    // Styleguide required for building the component.
    if (!$this->styleguide && !$styleguide) {
      return FALSE;
    }

    // Prefer the passed guide, if present.
    $this->styleguide = $styleguide ?: $this->styleguide;
    $this->section = $this->styleguide->getSection($this->name);

    // Simple properties.
    $title = $this->section->getTitle();

    // Variable names within template.
    $variables = $this->findVariables($title, $this->section->getMarkup());

    // Modifiers.
    $modifiers = array();
    foreach ($this->section->getModifiers() as $modifier) {
      $modifiers[] = $modifier->getName();
    }

    // Classes.
    //$classes = $this->section->getClassName();

    // Template.
    //$this->section->getMarkup();
    $template = $this->open($this->section->getMarkup());

    // Javascript dependency.
    // $js_filepath = $this->configs['path'] . '/' . $this->name . '/' . $this->name . '.js';
    // if (file_exists($js_filepath)) {
    //   $js = $js_filepath;
    // }

    // Save for retrevial next time, set objet properties.
    $data = array(
      'title' => $title ?: '',
      'template' => $template ?: '',
      'variables' => $variables,
      'modifiers' => $modifiers,
      //'classes' => $classes,
      //'js' => $js,
    );
    $this->save($data);
    $this->set($data);

    return TRUE;
  }


  /**
   * Discover component data variables.
   *
   * @todo Need to sanely handle various JSON structure,
   *       without too much config burdern. (See table-list.json)
   *
   * @return array
   */
  private function findVariables($title, $markupFile) {
    $vars = array();
    $filename = str_replace('hbs', 'json', $markupFile);

    // Open the JSON file (assignment test).
    if ($data = $this->open($filename)) {
      $jsonData = json_decode($data, TRUE);
      $first = current($jsonData);
      // Allow multi-value JSON variable declaration.
      if (gettype($first) === 'array' && key($first) === 0) {
        $jsonData = $first[0];
      }
      $vars = array_keys($jsonData) ?: array();
    }
    return $vars;
  }


  /**
   * File handler utility, until Parser is used for everything.
   *
   * @param $filename
   *
   * @return sting
   *   File contents with line breaks;
   */
  private function open($filename) {
    $filepath = $this->section->getFilePath() . '/'. $filename;
    if (file_exists($filepath)) {
      return file_get_contents($filepath);
    }
    else {
      if (!variable_get('componentize_supress_messages', FALSE) ||
          strpos(request_path(), 'admin/structure/componentize/') === 0) {
        drupal_set_message(t(
          'Component file missing: @file', array('@file' => $filepath)
        ), 'warning');
      }
      return FALSE;
    }
  }


  /**
   * Set: Allow alternate config storage options.
   *
   * @todo Save as entities.
   *
   * @param array $data
   */
  private function save($data) {
    switch ($this->configs['storage']) {
      case 'variable':
        variable_set('componentize_' . $this->namespace, $data);
        break;

      case 'cache':
        cache_set('componentize_' . $this->namespace, $data);
        break;
    }
  }


  /**
   * Get: Allow alternate config caching options.
   *
   * @todo Retrieve as entities.
   */
  private function retrieve() {
    $data = &drupal_static(__FUNCTION__ . $this->namespace);
    if ($data) return;

    switch ($this->configs['storage']) {
      case 'variable':
        $data = variable_get('componentize_' . $this->namespace);
        break;

      case 'cache':
        $data = cache_get('componentize_' . $this->namespace, 'cache');
        break;
    }

    $this->set($data);
  }


  /**
   * Set object properties from known data.
   *
   * @param array $set
   */
  private function set($data) {
    $this->title = isset($data['title']) ? $data['title'] : '';
    $this->template = isset($data['template']) ? $data['template'] : '';
    $this->variables = isset($data['variables']) ? $data['variables'] : array();
    $this->modifiers = isset($data['modifiers']) ? $data['modifiers'] : array();
  }
}
