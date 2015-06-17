<?php
/**
 * @file API documentation for component Fieldgroup.
 */


/**
 * Implements hook_entity_view_alter().
 *
 * @param array $build
 * @param string $type
 */
function hook_entity_view_alter(&$build, $type) {
  if ($type !== 'my_type') return;
    // Prepare workers.
    $component = new ComponentFactory('Section.ComponentizedField');
    $wrapper = entity_metadata_wrapper($type, $build);
    $field_handler = new ComponentFieldMyType($wrapper->field_componentized);
    // Push the data through plugibly.
    $entity['field_componentized'] = $component->render(
      $field_handler->getValues()
    );
  }
}


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
 *
 * @param array $handlers
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
