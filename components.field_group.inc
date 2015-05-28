<?php
/**
 * @file Fieldgroup management.
 */

define('COMPONENTS_LABEL_HIDDEN', 1);
define('COMPONENTS_LABEL_ABOVE', 2);
define('COMPONENTS_LABEL_CAPTION', 3);

/**
 * Implements hook_field_group_formatter_info().
 */
function components_field_group_formatter_info() {
  $formatters = array();

  return array(
    'form' => array(
      'component_element' => array(
        'label' => t('Component'),
        'description' => t("This fieldgroup renders the inner content using the selected Component's structure"),
        'instance_settings' => array(
          'component' => '',
          'modifiers' => '',
          'classes' => '',
        ),
      ),
    ),
    'display' => array(
      'component_element' => array(
        'label' => t('Component'),
        'descripton' => t("This fieldgroup renders the inner content using the selected Component's structure"),
        'instance_settings' => array(
          'component' => '',
          'modifiers' => '',
          'classes' => '',
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
function components_field_group_format_settings($group) {
  // Wrapper for extra settings to use by others.
  $form = array(
    'instance_settings' => array(
      '#tree' => TRUE,
      '#weight' => 5
  ));
  // Readable variables.
  $mode = ($group->mode === 'form') ? 'form' : 'display';
  $field_group_types = field_group_formatter_info();
  $formatter = $field_group_types[$mode][$group->format_type];

  // Add optional instance_settings.
  switch ($group->format_type) {
    case 'component_element':
     $form['instance_settings']['component'] = array(
        '#title' => t('Component'),
        '#type' => 'select',
        '#options' => array('none' => t('None'), 'button' => t('Button'), 'box' => t('Box')),
        '#default_value' => isset($group->format_settings['instance_settings']['component']) ?
            $group->format_settings['instance_settings']['component'] :
            $formatter['instance_settings']['component'],
        '#weight' => 2,
      );

      $form['instance_settings']['modifiers'] = array(
        '#title' => t('Modifiers'),
        '#type' => 'checkboxes',
        '#options' => array('none' => t('None'), 'green' => t('Green'), 'box' => t('Blue')),
        '#default_value' => isset($group->format_settings['instance_settings']['modifiers']) ?
            $group->format_settings['instance_settings']['modifiers'] :
            $formatter['instance_settings']['modifiers'],
        '#weight' => 2,
      );

      $form['instance_settings']['classes'] = array(
        '#title' => t('Extra CSS classes'),
        '#type' => 'textfield',
        '#default_value' => isset($group->format_settings['instance_settings']['classes']) ?
            $group->format_settings['instance_settings']['classes'] :
            $formatter['instance_settings']['classes'],
        '#weight' => 3,
        '#element_validate' => array('field_group_validate_css_class'),
      );

     /* $form['instance_settings']['expected_classes'] = array(
        '#title' => t('Expected Fields'),
        '#type' => 'markup',
        '#value' => 'placeholder',
        '#states' => array(
          'visible' => array(
            ':input[name=component]'
          ),
        ),
      ); */

      break;
  }

  return $form;
}


/**
 * Implements field_group_pre_render_<format-type>.
 *
 * @param array &$element
 * @param array $group
 * @param array &$form
 */
function field_group_pre_render_component_element(&$element, $group, &$form) {
  $settings = $group->format_settings['instance_settings'];

  $element += array(
    '#type' => 'component_fieldgroup',
    '#theme' => 'component_fieldgroup',
    '#title' => check_plain(t($group->label)),
    '#description' => $group->description,
    '#parents' => array($group->parent_name),
    '#children' => $group->children,
    '#settings' => $settings,
    '#mapping' => '@todo'
  );

dsm('@todo: include field to template mapping');
dsm('@todo: attach js dependencies');
  // $js[] = 'sites/all/components/' . $settings->component->dependencies . '.js';
  //$element['#attached']['js'][] = $js;
}
