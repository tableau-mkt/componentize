<?php
/**
 * @file API documentation for component fields.
 */


/**
 * Use entity field data (via easy field plugins) to render components.
 *
 * @param  [type] $variables [description]
 * @return [type]            [description]
 */
function theme_my_module_theme_something($variables) {
  // Load component, optionally set modifier.
  $component = ComponentFactory::create('Section.ComponentizedField');
  $component->setModifier($variables['element']['#settings']['modifier']);
  // Load field handler, get field values for template.
  $handler = new ComponentFieldMyType($variables['element']['field_myfield']);
  $template_vars = $handler->getValues($element[$field_name]['#items'][0]);
  // Render the component.
  $component->render($template_vars);
}


/**
 * Implements hook_componentize_field_types_info().
 */
function hook_componentize_field_types_info() {
  return array(
    'my_type' => 'ComponentFieldMyType',
  );
}


/**
 * Alter the list of field handlers.
 *
 * @param array $handlers
 */
function hook_componentize_field_types_info_alter($handlers) {
  $handlers['some_field_type'] = 'ComponentFieldType';
}


/**
 * Allow altering the component settings for an entity view mode.
 *
 * @param object $record
 *   Settings record to be stored.
 * @param $form_state
 */
function hook_entity_view_mode_component($record, &$form_state) {
  if ($record->bundle === 'my_bundle') {
    $record->modifier = 'force-this-modifier';
  }
}


/**
 * Sample field handler for moving CMS data to component template.
 *
 * See: lib/ComponentField.php
 */
class ComponentFieldMyType extends ComponentField {
  public function getValue($item) {
    // $this->field
    // $this->format
    // $this->type

    return array(
      'myProperty' => $item['my_property'],
      'specialInfo' => $item['special'],
    );
  }
}
