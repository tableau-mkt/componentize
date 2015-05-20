<?php
/**
 * @file Web component object for Drupal, uses Handlebars and SASS.
 */

class Component {

  public  $name,
          $namespace,
          $configs,
          $js,
          $template,
          $renderer,
          $variables,
          $modifiers;

  /**
   * Constructor.
   */
  public function __construct($name, $configs) {
    $this->name = $name;
    $this->configs = $configs;
    $this->namespace = $this->configs['module'] . '-' .  $this->$name;

    $data = $this->load();
    $this->template = $data['template'];
    $this->renderer = $data['renderer'];
    $this->variables = $data['variables'];
    $this->modifiers = $data['modifiers'];
    $this->js = $data['js'];
  }


  /**
   * Process data via template.
   *
   * @param array $data
   *
   * @return string
   */
  public function render($data) {
    return $this->renderer($data);;
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

    // Variable names in data file.
    $data = $this->openFile($this->$name . '.json');
    $variables = (json_decode($data, TRUE)) ?: array();

    // Modifiers from style file.
    $data = $this->openFile('_' . $this->$name . '.sass');
    $modifiers = $this->findModifiers($data);

    // Template from Handlebars file.
    $data = $this->openFile($this->$name . '.hbs');
    $handlebar = new LightnCandy();

    // Javascript dependency check.
    $js = file_exists(
      $this->configs['path'] . '/' . $this->$name . '/' . $this->$name . '.js'
    );

    // Prepage for storage and retrevial.
    $component_data = array(
      'template' => $data ?: FALSE,
      'renderer' => $handlebar->compile($data) ?: FALSE,
      'variables' => array_keys(drupal_json_decode($variables)),
      'modifiers' => $modifiers,
      'js' => $js,
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
  private function openFile($filename) {
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
   * Discover modifiers from style file.
   *
   * @param  $data
   *   Contents of the file.
   *
   * @return array
   */
  private function findModifiers($data) {
    $modifiers = array();
    foreach (explode("\n", $data) as $line) {
      if (strpos($line, 'Modifiers:') !== 0) {
        continue;
      }
      if (strpos($line, '.') === 0) {
        list($class, $description) = explode($line);
        $modifiers[$class] = $description;
      }
    }

    return $modifiers;
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

      default:
        return FALSE;
        break;
    }
  }


  /**
   * Get: Allow alternate config caching options.
   *
   * @todo Retrieve as entities.
   */
  private function retreive() {
    switch ($this->configs['storage']) {
      case 2:
        variable_get($this->namespace, $data);
        break;

      case 1:
        cache_get($this->namespace, $data);
        break;

      default:
        return FALSE;
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
