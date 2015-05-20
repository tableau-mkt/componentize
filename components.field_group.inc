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
 * @return array $form
 *   The form element for the format settings.
 */
function components_field_group_format_settings($group) {
   // Wrapper for extra settings to use by others.
  $form = array(
    'instance_settings' => array('#tree' => TRUE, '#weight' => 5),
  );

  $field_group_types = field_group_formatter_info();
  $mode = $group->mode == 'form' ? 'form' : 'display';
  $formatter = $field_group_types[$mode][$group->format_type];
dpm($group);

if ($mode == 'display') {
  //dpm(field_group_info_groups('node','article'));
  dpm($field_group_types);
}
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
 * Implements hook_field_group_pre_render().
 */
// function components_field_group_pre_render(&$element, $group, &$form) {

// dsm('PLEASE');

//   $element['#group'] = $group;

//   switch ($group->format_type) {
//     case 'component_element':

//       $items = array();
//       foreach ($group->children as $group_name) {
//         $items[$group_name] = &$element[$group_name];
//       }
//       $element['#items'] = $items;
//       break;

//     default:
//   }
// }


/**
 * Implements field_group_pre_render_<format-type>.
 * Format type: Component element.
 *
 * @param array &$element
 * @param array $group
 * @param array &$form
 */
function field_group_pre_render_component_element(&$element, $group, &$form) {
  $settings = $group->format_settings['instance_settings'];
  $mode = $group->mode == 'form' ? 'form' : 'display';

dsm('@todo: check mode');
dsm('@todo: attach dependencies');

dsm('RENDER-LAYER');
dsm($element);
dsm($group);
dsm($form);
dsm($mode);

  $element += array(
    '#type' => 'component_fieldgroup',
    '#theme' => 'component_fieldgroup',
    '#title' => check_plain(t($group->label)),
    '#description' => $group->description,
    '#parents' => array($group->parent_name),
    '#settings' => $settings,
  );

  // $js[] = 'sites/all/components/' . $settings->component->dependencies . '.js';
  //$element['#attached']['js'][] = $js;


  // $element += array(
  //   '#type' => 'markup',
  //   '#title' => check_plain(t($group->label)),
  //   '#pre_render' => array(),
  //   //'#attributes' => array('class' => explode(' ', $group->classes)),
  //   '#description' => $group->description,
  //   // Add fields within rendering.
  //   '#component_field_group' => array(
  //     '#theme' => 'components_fieldgroup',
  //     //'#theme' => "components_field_group__$group->group_name",
  //     //'#caption' => ($settings['label_visibility'] == COMPONENTS_LABEL_CAPTION) ? $label : NULL,
  //     '#attributes' => array(
  //       'class' => array('field-group-format', $group->group_name),
  //     ),
  //   ),
  // );

}


/**
 * Implements hook_field_group_pre_render().
 *
 * Create the component wrapper that can contains the fields.
 */
// function components_field_group_pre_render(&$element, $group, &$form) {
//   // Only process components field groups.
//   if ($group->format_type !== 'components') {
//     return;
//   }

//   $mode = $group->mode == 'form' ? 'form' : 'display';
//   $settings = $group->format_settings['instance_settings'];
//   $label = check_plain($group->label);

//   // Label and description of the group.
//   $caption = array();
//   if ($settings['label_visibility'] == COMPONENTS_LABEL_ABOVE) {
//     $caption = array(
//       '#type' => 'item',
//       '#title' => $label,
//       '#description' => field_filter_xss($settings['desc']),
//     );
//   }

//   // Create the element.
//   $element += array(
//     '#theme' => 'field_group_table_wrapper',
//     '#title' => $group->label,
//     '#mode' => $mode,
//     '#groups' => array_keys($form['#groups']),
//     '#settings' => $settings,
//     '#attributes' => array(
//       'class' => array_merge(array('field-group-table', $group->group_name), explode(' ', $settings['classes'])),
//     ),
//     '#caption' => $caption,
//     // We will add the table rows upon rendering, as doing it here means
//     // messing up the field group hierarchy, which causes issues.
//     '#field_group_table' => array(
//       '#theme' => "table__field_group_table__$group->group_name",
//       '#header' => $header,
//       '#caption' => ($settings['label_visibility'] == FIELD_GROUP_TABLE_LABEL_CAPTION) ? $label : NULL,
//       '#attributes' => array(
//         'class' => array('field-group-format', $group->group_name),
//       ),
//     ),
//   );
// }
