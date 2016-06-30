<?php
/**
 * @file API documentation for Componentize.
 */


/**
 * Use a component to render content.
 */
function my_module_block_view() {
  $component = ComponentFactory::create('Section.Component');
  $component->render(array(
    'name' => 'Smarty Pants',
    'score' => '99',
  ));
}


/**
 * Add custom components.
 *
 * @param array &$list
 *   List of components known about by the site.
 * @param boolean $shallow
 *   Either return an array of strings or objects.
 *
 * @see componentize_list()
 */
function hook_componentize_list_alter(&$list, $shallow) {
  if ($shallow) {
    // Just add yours to the array.
    $list[] = 'my_component';
    return;
  }

  // Build a full component.
  $configs = array(
    'path'    => drupal_get_path('module', 'my_module') . '/components',
    'module'  => 'my_module',
    'css'     => 'components/my-component.css',
    'js'      => 'components/my-component.js',
    'storage' => 'full', // Optional: storage level (variable, full, none).
  );
  $list['my_component'] = ComponentFactory::create('Section.Component', $configs);

  // Mess with an existing component.
  if (current_path() === 'my-special-path') {
    $list['some_component']->setModifier('.my-special-modifier');
  }
}


/**
 * Alter data passed to the template.
 *
 * @param Component $component
 *   Component object about to be rendered.
 * @param array &$data
 *   Key/value array of template variables.
 */
function hook_componentize_render_alter(&$component, &$data) {
  if ($component->getName() === 'my_component') {
    $data['special_text'] = t('My Special Text');
  }

  if (path_is_admin(current_path())) {
    $data['admin_mode'] = 'admin';
  }
}

/**
 * Add a custom handlebars helper function
 *
 * @return array
 *   Associative array of helper name and custom helper function
 *
 * @see https://github.com/zordius/lightncandy#custom-helper
 */
function hook_componentize_helpers_info() {
  $helpers = array(
    "helper-name" => "my_module_helper_function",
  );

  return $helpers;
}


/**
 * Example custom handlebars helper function.
 *
 * @param array $context
 *   Array of the data passed into the helper.
 * @param array $options
 *   Options array passed into the helper.
 * @return array
 *   The final output to go to the template.
 *
 * @see https://github.com/zordius/lightncandy#custom-helper
 */
function my_module_helper_function($context, $options) {
  // Helper to render an array as HTML attributes.
  return drupal_attributes($context[0]);
}
