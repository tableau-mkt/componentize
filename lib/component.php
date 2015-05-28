<?php
/**
 * @file Web component object for Drupal, uses Handlebars and SASS.
 */

namespace Component;

use Scan\Kss\Parser;
use LightnCandy;

class Component {

  private $name,
          $configs,
          $namespace,
          $template,
          $variables,
          $modifiers;

  /**
   * Constructor.
   */
  public function __construct($name, $configs = array(), $styleguide = FALSE) {
    $this->name = $name;

    // Honor passed, provide a default.
    $this->configs = $configs + array(
      'path' => variable_get('components_directory', COMPONENTS_DIRECTORY),
      'storage' => variable_get('components_storage', 1),
      'module' => 'components',
    );

    // Full storage namespace.
    $this->namespace = $this->configs['module'] . '-' .  $this->name;
    // Use a common styleguide parser if provided.
    // @todo Share the styleguide between instances.
    $this->styleguide = $styleguide ?: new Parser($this->configs['path']);

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
    $path = 'public://components';
    if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      drupal_set_message(t('Unable to create Components template cache directory. Check the permissions on your files directory.'), 'error');
      return;
    }

    // Stash compiled (PHP) version of template.
    // @todo Allow for private files.
    $filepath = 'public://components/' . $this->namespace . '.php';
    if (!file_exists($filepath)) {
      $handlebar = new LightnCandy();
      $compiled = $handlebar->compile($this->template);
      file_unmanaged_save_data($compiled, $filepath, FILE_EXISTS_REPLACE);
    }
    $renderer = include($filepath);
    return $renderer($data);
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

    // Variable names within template.
    $data = $this->openComponent($shortName . '/' . $shortName . '.json');
    $variables = array_keys(json_decode($data, TRUE)) ?: array();

    // Modifiers.
    $section = $this->styleguide->getSection($this->name);
    $modifiers = $section->getModifiers();

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
