<?php
/**
 * @file API documentation for Components.
 */


/**
 * Add custom components.
 *
 * @param array &$list
 *   List of components known about by the site.
 * @param boolean $shallow
 *   Either return an array of strings or objects.
 *
 * @see _components_list_alter()
 */
function hook_components_list_alter(&$list, $shallow) {
  if ($shallow) {
    $list[] = 'my_component';
  }

  $configs = array(
    'path' => drupal_get_path('module', 'my_module') . '/components',
    'module' => 'my_module',
    'css' => 'components/my-component.js',
    'css' => 'components/my-component.css',
    'storage' => 'full', // Optional: storage level (variable, full, none).
  );
  $list['my_component'] = new ComponentFactory('Section.Component', $configs);
}


/**
 * Alter data passed to the template.
 *
 * @param Component $component
 *   Component object about to tbe rendered.
 * @param array &$data
 *   Key/value array of template variables.
 */
function hook_components_render($component, &$data) {
  if ($component->getName() === 'my_component') {
    $data['special_text'] = t('My Special Text');
  }

  if (path_is_admin(current_path())) {
    $data['admin_mode'] = 'admin';
  }
}



/**
 * Use a component to render code.
 */
function my_module_block_view() {
  $component = new Component('Section.Component');
  $component->render(array(
    'name' => 'Smarty Pants',
    'score' => '99',
  ));
}
