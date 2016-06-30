<?php
/**
 * @file Component object for Drupal.
 *
 * Uses KSS (Handlebars and SASS/CSS comments, etc).
 */

namespace Componentize;
use Componentize\ParserKSSnode;

class ComponentFactory {

  private static $styleguide;

  // Prevent building new factories.
  protected function __construct() {
    // Access via ComponentFactory::create().
  }

  /**
   * Generate a component.
   *
   * @param string $name
   * @param array $configs
   *
   * @return Component
   */
  public static function create($name, $configs = array()) {
    // Honor passed, provide a default.
    $configs = $configs + array(
      'path' => variable_get('componentize_directory', COMPONENTIZE_DIRECTORY),
      'partials' => variable_get('componentize_partials', COMPONENTIZE_PARTIALS),
      'storage' => variable_get('componentize_storage', 'none'),
      'module' => 'componentize',
      'reset' => FALSE,
    );

    // Attempt to get component.
    $component = new Component($name, $configs);

    // Build a style guide and load component if not yet stored.
    // Component handles reset.  Template is a proxy for built status.
    if (!property_exists($component, 'template') || empty($component->template)) {
      self::setGuide($configs['path']);
      $component->load(self::$styleguide);
    }

    return $component;
  }


  /**
   * Set the factory style guide. Avoids building when unecessary.
   */
  private static function setGuide($path) {
    // Use a common static styleguide Parser.
    if (!self::$styleguide) {
      self::$styleguide =  new ParserKSSnode($path);
    }
  }

}
