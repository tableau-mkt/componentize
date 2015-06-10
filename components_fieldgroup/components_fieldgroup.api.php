<?php
/**
 * @file API documentation for Components Fieldgroup.
 */


/**
 * Implements hook_components_fieldgroup_field_types_info().
 */
function hook_components_fieldgroup_field_types_info() {
  return array(
    'my_type' => 'ComponentFieldMyType',
  );
}


/**
 * Alter the list of field handlers.
 */
function hook_components_fieldgroup_field_types_info_alter($handlers) {
  $handlers['some_field_type'] = 'ComponentsFieldAltHandler';
}


/**
 * Sample field handler for moving CMS data to component template.
 *
 * See: ComponentField.
 */
class ComponentsFieldMyType extends ComponentsField {
  public function getValue($item) {
    return array(
      'myProperty' => $item['my_property'],
      'specialInfo' => $item['special'],
    );
  }
}
