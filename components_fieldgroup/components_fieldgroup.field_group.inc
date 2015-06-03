<?php
/**
 * @file Fieldgroup management.
 */

define('COMPONENTS_LABEL_HIDDEN', 1);
define('COMPONENTS_LABEL_ABOVE', 2);
define('COMPONENTS_LABEL_CAPTION', 3);

/**
 * Implements hook_field_group_formatter_info().
 *
 * @todo Avoid showing up in form admin.
 */
function components_fieldgroup_field_group_formatter_info() {
  return array(
    'display' => array(
      'components_fieldgroup' => array(
        'label' => t('Component'),
        'descripton' => t("This fieldgroup renders the inner content using the selected Component's structure"),
        'instance_settings' => array(
          'component' => '',
          'modifiers' => '',
        ),
      ),
    ),
  );
}


/**
 * Implements hook_field_group_format_settings().
 *
 * @params object $group
 *   The group object.
 *
 * @return array $form
 *   The form element for the format settings.
 */
function components_fieldgroup_field_group_format_settings($group) {
  $mode = ($group->mode === 'form') ? 'form' : 'display';
  if ($mode === 'form') return;

  $field_group_types = field_group_formatter_info();
  //$formatter = $field_group_types[$mode][$group->format_type];

  // Wrapper for extra settings to use by others.
  $form = array('instance_settings' => array('#tree' => TRUE, '#weight' => 5));

  // Add optional instance_settings.
  switch ($group->format_type) {
    case 'components_fieldgroup':

      $form['instance_settings']['component'] = array(
        '#title' => t('Component'),
        '#type' => 'select',
        '#options' => drupal_map_assoc(_components_list(TRUE)),
        '#default_value' => _components_fieldgroup_settings_default($group, 'component'),
        '#weight' => 1,
      );

      $form['instance_settings']['modifier'] = array(
        '#title' => t('Modifier'),
        '#type' => 'fieldgroup',
        '#element_validate' => array('components_fieldgroup_single_modifier_validate'),
        '#weight' => 2,
      );
      foreach (_components_list() as $component) {
        //Separate list for each components' modifiers.
        $form['instance_settings']['modifier'][$component->name] = array(
          '#title' => $component->name . ' Modifier',
          '#type' => 'select',
          '#options' => drupal_map_assoc($component->getModifiers()),
          '#states' => array(
            'visible' => array(
              ':input[name$="[component]"]' => array('value' => $component->name),
            ),
          ),
          '#default_value' => _components_fieldgroup_settings_default($group, 'modifier'),
        );
      }

      break;
  }

  return $form;
}


/**
 * Get the settings detault.
 */
function _components_fieldgroup_settings_default($group, $property) {
  if (isset($group->format_settings['instance_settings'][$property])) {
    return $group->format_settings['instance_settings'][$property];
  }
  return '';
}


/**
 * Implements field_group_pre_render_<format-type>.
 * Format: components_fieldgroup
 *
 * @param array &$element
 * @param array $group
 * @param array &$form
 */
function field_group_pre_render_components_fieldgroup(&$element, $group, &$form) {
  // @todo May want to access the label position within the template.
  // $element['#label_display']

  // Add necessary items to variables within theme function.
  $element += array(
    '#type' => 'components_fieldgroup',
    '#theme' => 'components_fieldgroup',
    '#title' => t($group->label),
    '#description' => $group->description,
    '#parents' => array($group->parent_name),
    '#children' => $group->children,
    '#settings' => $group->format_settings['instance_settings'],
    '#view_mode' => $group->mode,
  );
}