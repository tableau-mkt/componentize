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
  $form = array('instance_settings' => array('#tree' => TRUE, '#weight' => 5));

  // Add optional instance_settings.
  switch ($group->format_type) {
    case 'component_fieldgroup':

      $form['instance_settings']['component'] = array(
        '#title' => t('Component'),
        '#type' => 'select',
        '#options' => drupal_map_assoc(componentize_list(TRUE)),
        '#default_value' => componentize_fieldgroup_settings_default($group, 'component'),
        '#weight' => 1,
      );

      $form['instance_settings']['modifier'] = array(
        '#title' => t('Modifier'),
        '#type' => 'fieldgroup',
        '#element_validate' => array('componentize_fieldgroup_single_modifier_validate'),
        '#weight' => 2,
      );
      foreach (componentize_list() as $component) {
        // Separate list for each components' modifiers.
        $form['instance_settings']['modifier'][$component->getName()] = array(
          '#title' => $component->getName() . ' Modifier',
          '#type' => 'select',
          '#options' => array_merge($component->getModifiers(), array('none' => 'none')),
          '#states' => array(
            'visible' => array(
              ':input[name$="[component]"]' => array('value' => $component->getName()),
            ),
          ),
          '#default_value' => componentize_fieldgroup_settings_default($group, 'modifier'),
        );
      }

      break;
  }

  return $form;
}


/**
 * Get the settings detault.
 */
function componentize_fieldgroup_settings_default($group, $property) {
  if (isset($group->format_settings['instance_settings'][$property])) {
    return $group->format_settings['instance_settings'][$property];
  }
  return '';
}
