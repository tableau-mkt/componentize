<?php
/**
 * @file Browse available component structure.
 */

/**
 * Menu callback for viewing available components.
 */
function _components_library() {
  foreach (_components_list() as $component) {
    $output[] = $component->name;
  }

  drupal_theme('table', $output);
}
