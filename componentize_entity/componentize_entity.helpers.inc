<?php
/**
 * @file Helper functions: Componentize! Entity.
 */

define('COMPONENTIZE_EVM_TABLE', 'componentize_entity_entity_view_mode');

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

  $settings = &drupal_static(__FUNCTION__);
  if (!$settings) {
    // Get all values from custom schema table.
    $settings = ctools_export_crud_load_all('componentize_entity_entity_view_mode');
  }

  if (isset($settings[$export_id])) {
    return $settings[$export_id];
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
  $entity_uri = entity_uri($entity_type, $entity);
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

  // Make paths available.
  $vars['url'] = url($entity_uri['path'], $entity_uri['options']);
  $vars['url_absolute'] = url($entity_uri['path'], $entity_uri['options'] + array('absolute' => TRUE));

  // Add default entity properties.
  foreach ($copy_props as $name) {
    if (!isset($entity->{$name})) {
      continue;
    }
    $vars[$name] = $entity->{$name};
  }

  // Add mapped fields.
  foreach ($fields as $name => $field) {
    $settings = _componentize_entity_get_field_settings($field, $view_mode);

    // Only include componentized fields.
    if ($settings) {
      // Prepare data for rendering.
      $items = field_get_items($entity_type, $entity, $name);

      // Build out template data.
      $template_var = $settings[COMPONENTIZE_TEMPLATE_VAR];

      // Keep an array of unique keys to cross reference for duplicates.
      $template_vars[] = $template_var;
      $keys = _componentize_unflatten_variable($vars, $template_vars, $template_var);
      $temp = &$vars;
      $i = 0;
      $count = count($keys);

      foreach($keys as $key) {
        if (++$i === $count) {
          $temp[$key . '_label'] = $field['label'];
          $temp[$key] = componentize_entity_render_field(
            $name, $entity_type, $entity, $view_mode, $items, $settings
          );
          break;
        }

        $temp = &$temp[$key];
      }
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

  // Return view mode settings.
  if (isset($display[$view_mode]) && !empty($display[$view_mode]['settings'][COMPONENTIZE_TEMPLATE_VAR])) {
    return $display[$view_mode]['settings'];
  }
  // Return default settings.
  if (!empty($display['default']['settings'][COMPONENTIZE_TEMPLATE_VAR])) {
    return $display['default']['settings'];
  }

  return FALSE;
}


/**
 * Build field settings summary shared function.
 *
 * @param string &$summary
 * @param array $field_settings
 */
function _componentize_entity_generate_summary(&$summary, $field_settings) {
  if (!empty($summary)) {
    $summary .= '<br />';
  }

  // Display settings.
  if (!empty($field_settings[COMPONENTIZE_TEMPLATE_VAR])) {
    $summary .= t('Map to "@variable" template variable', array(
      '@variable' => $field_settings[COMPONENTIZE_TEMPLATE_VAR],
    ));
    $summary .= '<br />';

    if ($field_settings[COMPONENTIZE_TEMPLATE_HANDLER]) {
      $summary .= t('Using field handler for template values');
    }
    else {
      $summary .= t('Rendered prior to template');
    }
  }
  else {
    $summary .= t('No template variable mapped yet!');
  }
}


/**
 * Add variable choices for registered component.
 *
 * @todo Set componentize_fieldgroup module weight higher and
 *       run the duplicate form element check the other way.
 *
 * @param array &$form
 * @param string $component
 * @param array $display
 */
function _componentize_entity_generate_variable_form(&$form, $component, $display) {
  // Allow other modules to borrow this.
  if (!isset($form[COMPONENTIZE_TEMPLATE_VAR])) {
    // Add variables.
    $form[COMPONENTIZE_TEMPLATE_VAR] = array(
      '#title' => t('Component Variable'),
      '#type' => 'select',
      '#options' => array_merge(
        array('' => t('- None -')),
        $component->getVariables()
      ),
      '#default_value' => $display[COMPONENTIZE_TEMPLATE_VAR],
    );
    $form[COMPONENTIZE_TEMPLATE_HANDLER] = array(
      '#title' => t('Data through handler'),
      '#type' => 'checkbox',
      '#description' => t('Skip Drupal rendering and send data to template via handler plugin.'),
      '#default_value' => $display[COMPONENTIZE_TEMPLATE_HANDLER],
    );
  }
}


/**
 * Remove existing settings for disabled view modes.
 *
 * @param array &$form
 * @param array $settings
 */
function _componentize_entity_remove_disabled_evm(&$form, $settings) {
  if (empty($settings)) {
    return FALSE;
  }

  foreach ($settings as $view_mode => $active) {
    if ($active) {
      continue;
    }

    $remove_id = implode('|', array(
      $form['#entity_type'],
      $form['#bundle'],
      $view_mode
    ));

    $query = 'SELECT id from {' . COMPONENTIZE_EVM_TABLE . '} WHERE id = :id';
    $exists = db_query($query, array(':id' => $remove_id))->rowCount();

    if ($exists) {
      db_delete(COMPONENTIZE_EVM_TABLE)
        ->condition('id', $remove_id)
        ->execute();
    }
  }
}
