<?php
/**
 * @file Web component object for Drupal, uses Handlebars and SASS.
 */

namespace Component;
use Component\ParserKSSnode;

class ComponentFactory {

  private static $styleguide;
  private $configs;

  public function create($name, $configs = array()) {

    // Honor passed, provide a default.
    $this->configs = $configs + array(
      'path' => variable_get('components_directory', COMPONENTS_DIRECTORY),
      'storage' => variable_get('components_storage', 1),
      'module' => 'components',
      'reset' => FALSE,
    );

    // @todo Avoid even creating the styleguide if everything is already in storage.

    // Use a common static styleguide parser.
    if (!self::$styleguide) {
      self::$styleguide =  new ParserKSSnode($this->configs['path']);
    }

    return new Component($name, $this->configs, self::$styleguide);
  }
}
