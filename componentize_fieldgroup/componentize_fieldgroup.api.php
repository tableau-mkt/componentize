<?php
/**
 * @file API documentation for component Fieldgroup.
 */


/**
 * Implements hook_componentize_fieldgroup_field_types_info().
 */
function hook_componentize_fieldgroup_field_types_info() {
  return array(
    'my_type' => 'ComponentFieldMyType',
  );
}


/**
 * Alter the list of field handlers.
 */
function hook_componentize_fieldgroup_field_types_info_alter($handlers) {
  $handlers['some_field_type'] = 'ComponentFieldType';
}


/**
 * Sample field handler for moving CMS data to component template.
 *
 * See: lib/ComponentField.php
 */
class ComponentFieldMyType extends ComponentField {
  public function getValue($item) {
    return array(
      'myProperty' => $item['my_property'],
      'specialInfo' => $item['special'],
    );
  }
}
