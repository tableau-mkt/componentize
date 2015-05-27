<?php
/**
 * @file Web component object for Drupal, uses Handlebars and SASS.
 */

namespace Component;

use Scan\Kss\Parser;
use LCRun3\LightnCandy;

class Component {

  public  $name,
          $configs,
          $namespace,
          $markup,
          $variables,
          $modifiers;

  /**
   * Constructor.
   */
  public function __construct($name, $configs = array(), $styleguide = FALSE) {
    $this->name = $name;

    // Honor passed, but ensure a value.
    $defaults = array(
      'storage' => variable_get('components_storage', 2),
      'path' => variable_get('components_directory', COMPONENTS_DIRECTORY),
      'module' => 'components',
    );
    foreach ($defaults as $key => $default) {
      $configs[$key] = isset($configs[$key]) ? $configs[$key] : $default;
    }
    $this->configs = $configs;

    // Full storage namespace.
    $this->namespace = $this->configs['module'] . '-' .  $this->name;
    // Use a common styleguide parser if provided.
    $this->styleguide = $styleguide ?: new Parser($this->configs['path']);

    // Build out component.
    $data = $this->load();
    $this->template = $data['markup'];
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
    $handlebar = new LightnCandy();
    $handle = $handlebar->compile($this->template);
    return $handle->renderer($data);
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

    // Variable names within template.
    $data = $this->openComponent($this->name . '.json');
    $variables = array_keys(json_decode($data, TRUE)) ?: array();

    // Modifiers.
    $modifiers = $section->getModifiers();
    // $data = $this->openFile('_' . $this->name . '.scss');
    // @todo Find markup via SASS label (Markup: template.hbs)
    // $modifiers = $this->findModifiers($data);

    // Classes.
    $classes = $modifier->getClassName();

    // Custom component javascript dependency check.
    // $js_filepath = $this->configs['path'] . '/' . $this->name . '/' . $this->name . '.js';
    // if (file_exists($js_filepath)) {
    //   $js = $js_filepath;
    // }

    // Prepage for storage and retrevial.
    $component_data = array(
      'template' => $section->getMarkup(),
      //'renderer' => $handlebar->compile($data) ?: FALSE,
      'variables' => $variables,
      'modifiers' => $modifiers,
      'classes' => $classes,
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
    $filepath = $this->configs['path'] . '/components/'. $filename;
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
   * Discover modifiers from style file.
   *
   * @param  $data
   *   Contents of the file.
   *
   * @return array
   */
  // private function findModifiers($data) {
  //   $modifiers = array();
  //   foreach (explode("\n", $data) as $line) {
  //     if (strpos($line, 'Modifiers:') !== 0) {
  //       continue;
  //     }
  //     if (strpos($line, '.') === 0) {
  //       list($class, $description) = explode($line);
  //       $modifiers[$class] = $description;
  //     }
  //   }

  //   return $modifiers;
  // }


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
        variable_get($this->namespace, array());
        break;

      case 1:
        cache_get($this->namespace, array());
        break;

      default:
        return array();
        break;
    }
  }


  /**
   * Set: Remove all stored records.
   *
   * @todo Delete entities.
   */
  private function delete() {
    variable_del($this->namespace, $data);
  }
}
