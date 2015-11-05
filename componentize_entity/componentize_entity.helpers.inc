<?php
/**
 * @file Helper functions: Componentize! Fields.
 */

require_once drupal_get_path('module', 'ctools') . '/includes/export.inc';

/**
 * List available field types for variables.
 *
 * @return array
 */
function componentize_entity_field_types() {
  $handlers = &drupal_static(__FUNCTION__);

  if (!$handlers) {
    $handlers = module_invoke_all('componentize_field_types_info');
    drupal_alter('componentize_field_types_info', $handlers);
  }

  return $handlers;
}


/**
 * Shared utility to find bundles with view modes.
 *
 * @return array
 */
function _componentize_entity_get_bundles_with_view_modes() {
  $list = &drupal_static(__FUNCTION__, array());
  if (!empty($list)) {
    return $list;
  }

  // Check all the entity types.
  foreach (entity_get_info() as $type => $type_info) {
    if (isset($type_info['view modes']) && count($type_info['view modes']) > 0) {
      foreach ($type_info['bundles'] as $bundle => $bundle_info) {
        // NOTE: entity type details added to each bundle array element.
        $list[$bundle] = $bundle_info + array(
          'entity_type' => $type,
          'entity_type_label' => $type_info['label'],
        );
      }
    }
  }

  return $list;
}


/**
 * Create vertical tab space for admin settings.
 *
 * @param array $form
 */
function _componentize_entity_ui_create_vertical_tabs(&$form) {
  // Add additional settings vertical tab.
  if (!isset($form['additional_settings'])) {
    $form['additional_settings'] = array(
      '#type' => 'vertical_tabs',
      '#theme_wrappers' => array('vertical_tabs'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
    $form['#attached']['js'][] = 'misc/form.js';
    $form['#attached']['js'][] = 'misc/collapse.js';
  }
  // Add layouts form.
  $form['additional_settings']['componentize'] = array(
    '#type' => 'fieldset',
    '#title' => t('Components'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#parents' => array('additional_settings'),
    '#tree' => TRUE,
    '#weight' => 10,
  );
}


/**
 * Get the current view mode settings.
 *
 * @param string $export_id
 *   Composite string of entity_type|bundle|view_mode.
 *
 * @return array
 */
function _componentize_entity_get_view_mode_display_settings($entity_type, $bundle, $view_mode) {
  $export_id  = implode('|', array($entity_type, $bundle, $view_mode));
  $default_id = implode('|', array($entity_type, $bundle, 'default'));

  $settings = &drupal_static(__FUNCTION__);
  if (!$settings) {
    // Get all values from custom schema table.
    $settings = ctools_export_crud_load_all('componentize_entity_entity_view_mode');
  }

  if (isset($settings[$export_id])) {
    return $settings[$export_id];
  }
  else if (isset($settings[$default_id])) {
    return $settings[$default_id];
  }

  return FALSE;

  // Could be handle to allow getting several settings at once, not currently necessary.
  // Quick return if exact.
  // if ($view_mode) {
  //   return isset($settings[$entity_type . '-' . $bundle . '-' . $view_mode] ?: FALSE;
  // }
  // // Find relevant and become nested array.
  // $match = $entity_type;
  // $match += $bundle ? '-' . $bundle : '';
  // foreach ($settings as $key => $config) {
  //   if (!preg_match('/^' . $match . '/', $config)) {
  //     list($e, $b, $view_mode) = explode('-', $config);
  //     $return[$e][$b][] = $view_mode;
  //   }
  // }
}


/**
 * Get relevant field data in display mode for rendering component.
 *
 * @todo Get fieldgroups!
 * @todo Get fieldgroups rendered as entities.
 * @todo Allow fields a components.
 *
 * @param array $build
 *
 * @return array
 *   Associative list of template data.
 */
function _componentize_entity_get_fields($build) {
  $view_mode = $build['#view_mode'];
  $fields = field_info_instances($build['#entity_type'], $build['#bundle']);
  $entity_type = $build['#entity_type'];
  $entity = isset($build['#entity']) ? $build['#entity'] : $build['#' . $entity_type];
  $vars = array();

  // Copy a bunch of entity properties into our $vars array.
  $copy_props = array(
    'type',
    'nid',
    'tnid',
    'title',
    'status',
    'promote',
    'sticky',
    'language',
    'translate',
    'changed',
    'comment_count',
    'created',
    'last_comment_timestamp',
    'last_comment_uid',
    'name',
    'metatags',
    'path',
    'revision_timestamp',
    'revision_uid',
  );

  // Prefix each property name (namespaced) for avoiding collisions.
  foreach ($copy_props as $name) {
    if (!isset($entity->{$name})) {
      continue;
    }
    $vars['entity_' . $name] = $entity->{$name};
  }

  foreach ($fields as $name => $field) {
    $settings = _componentize_entity_get_field_settings($field, $view_mode);

    // Only include componentized fields.
    if ($settings) {
      // Prepare data for rendering.
      $items = field_get_items($entity_type, $entity, $name);

      // Build out template data.
      $template_var = $settings['componentize_entity_variable_name'];
      $vars[$template_var . '_label'] = $field['label'];
      $vars[$template_var] = componentize_entity_render_field(
        $name, $entity_type, $entity, $items, $settings
      );
    }
  }

  // Allow modules to alter data with hook_componentize_entity_data_alter().
  drupal_alter('componentize_entity_data', $vars, $entity);

  return $vars;
}


/**
 * Grab appropriate view mode settings, allow default.
 *
 * @param array $display
 * @param string $view_mode
 *
 * @return array|boolean
 */
function _componentize_entity_get_field_settings($field, $view_mode) {
  $display = $field['display'];

  if (isset($display[$view_mode]) &&
      isset($display[$view_mode]['settings']['componentize_entity_variable_name']) &&
      !empty($display[$view_mode]['settings']['componentize_entity_variable_name'])) {
    return $display[$view_mode]['settings'];
  }

  if (isset($display['default']['settings']['componentize_entity_variable_name']) &&
      !empty($display['default']['settings']['componentize_entity_variable_name'])) {
    return $display['default']['settings'];
  }

  return FALSE;
}
