<?php
/**
 * @file Field handler: default text.
 */

namespace Componentize;

class ComponentField {

  public $type,
         $format,
         $field;

  /**
   * Build field data.
   *
   * @todo Alter params to match entity view scenarios.
   *       See: componentize_fieldgroup.api.php
   */
  public function __construct($field) {
    $this->field = $field;
    $this->format = $field['#formatter'];
    $this->type = $field['#field_type'];
  }

  /**
   * Plugable: obtain variables from field value(s).  Allows more complex fields.
   *
   * @param array $item
   *   Field value array.
   *
   * @return string|array
   *   Variable data to send to template.
   */
  public function getValues($item) {
    return $item['safe_value'];
  }

}
