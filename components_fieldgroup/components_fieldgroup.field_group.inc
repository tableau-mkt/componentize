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
  $formatters = array();

  return array(
    'form' => array(
      'components_fieldgroup' => array(
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
      'components_fieldgroup' => array(
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
function components_fieldgroup_field_group_format_settings($group) {
  $mode = ($group->mode === 'form') ? 'form' : 'display';
  if ($mode === 'form') return;

  $field_group_types = field_group_formatter_info();
  $formatter = $field_group_types[$mode][$group->format_type];

  // Wrapper for extra settings to use by others.
  $form = array(
    'instance_settings' => array(
      '#tree' => TRUE,
      '#weight' => 5
  ));

  // Add optional instance_settings.
  switch ($group->format_type) {
    case 'components_fieldgroup':

      $form['instance_settings']['component'] = array(
        '#title' => t('Component'),
        '#type' => 'select',
        '#options' => drupal_map_assoc(_components_list(TRUE)),
        '#default_value' => _components_fieldgroup_settings_defaults($group, $formatter, 'component'),
        '#weight' => 1,
      );
      $form['instance_settings']['modifiers'] = array(
        '#title' => t('Modifier'),
        '#type' => 'fieldgroup',
      );
      foreach (_components_list() as $component) {

//dsm(get_class_methods($component));

        // Separate list for each components' modifiers.
        // $form['instance_settings']['modifiers'][$component->name] = array(
        //   '#title' => $component->name . ' Modifiers',
        //   '#type' => 'select',
        //   '#options' => drupal_map_assoc($component->getModifiers()),
        //   '#states' => array(
        //     'visible' => array(
        //       ':input[name$="[component]"]' => array('value' => $component->name),
        //     ),
        //   ),
        //   '#default_value' => _components_fieldgroup_settings_defaults($group, $formatter, 'modifier', ''),
        // );
      }

      $form['instance_settings']['classes'] = array(
        '#title' => t('Extra CSS classes'),
        '#type' => 'textfield',
        '#default_value' => _components_fieldgroup_settings_defaults($group, $formatter, 'classes'),
        '#weight' => 3,
        '#element_validate' => array('field_group_validate_css_class'),
      );

      // @todo Add field mapping.

     /* $form['instance_settings']['variables'] = array(
        '#title' => t('Expected Fields'),
        '#type' => 'markup',
        '#value' => 'placeholder',
        '#states' => array(
          'visible' => array(
            ':input[name=component]'
          ),
        ),
      );
      */

      break;
  }

  return $form;
}


/**
 * Get the settings detault
 */
function _components_fieldgroup_settings_defaults($group, &$formatter, $property) {
  if (isset($group->format_settings['instance_settings'][$property])) {
    return $group->format_settings['instance_settings'][$property];
  }
  else {
    return $formatter['instance_settings'][$property];
  }
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
  $settings = $group->format_settings['instance_settings'];

  $element += array(
    '#type' => 'components_fieldgroup',
    '#theme' => 'components_fieldgroup',
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


/**
 * Helper function to parse the components directory
 */
function _components_fieldgroup_modifiers_options() {
  $dummy_components = array(
    'button' => array(
      'expected_fields' => array('title','description'),
      'modifiers' => array('cta','red','blue','yellow','small','large'),
    ),
    'box' => array(
      'expected_fields' => array('content'),
      'modifiers' => array('solid','outline','rounded','double-padding'),
    ),
    'badge' => array(
      'expected_fields' => array('title','number','tagline'),
      'modifiers' => array('red','blue','yellow','round'),
    ),
  );

  $components = array();

  foreach ($dummy_components as $key => $value) {
    $myComponent = new Component('badge');
    foreach ($value['expected_fields'] as $field) {
      $myComponent->addField($field);
    }
    foreach ($value['modifiers'] as $modifier) {
     $myComponent->addModifier($modifier);
    }
    $components[] = $myComponent;
  }

  return $components;
}
