<?php
/**
 * @file Field handler: default.
 */

namespace Componentize;

class ComponentField {

  public $type,
         $field_info;

  /**
   * Build field data.
   *
   * @todo Alter params to match entity view scenarios.
   *       See: componentize_fieldgroup.api.php
   */
  public function __construct($field_info) {
    $this->field_info = $field_info;
    $this->type = $field_info['type'];
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
    return $item;
  }

}
