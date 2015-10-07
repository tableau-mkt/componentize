<?php
/**
 * @file Fieldgroup settings management.
 */


/**
 * Implements hook_field_group_format_settings().
 *
 * @params object $group
 *   The group object.
 *
 * @return array $form
 *   The form element for the format settings.
 */
function componentize_fieldgroup_field_group_format_settings($group) {
  $mode = ($group->mode === 'form') ? 'form' : 'display';
  if ($mode === 'form') return;

  $field_group_types = field_group_formatter_info();
  //$formatter = $field_group_types[$mode][$group->format_type];

  // Wrapper for extra settings to use by others.
  $form = array('instance_settings' => array(
    '#tree' => TRUE,
    '#weight' => 5
  ));

  // Add optional instance_settings.
  if ($group->format_type === 'component_fieldgroup') {
    // Add modifier select form element.
    _componentize_build_modifier_list_form(
      $form,
      array('instance_settings'),
      _componentize_fieldgroup_display_settings_current($group, 'component'),
      _componentize_fieldgroup_display_settings_current($group, 'component_modifier')
    );
    $form['instance_settings']['component']['#element_validate'][] =
        'componentize_fieldgroup_single_modifier_validate';
  }

  return $form;
}


/**
 * Get the detault settings within fieldgroups only.
 */
function _componentize_fieldgroup_display_settings_current($group, $property) {
  if (isset($group->format_settings['instance_settings'][$property])) {
    return $group->format_settings['instance_settings'][$property];
  }
  return '';
}
