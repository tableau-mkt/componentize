<?php
/**
 * @file Field handler: default text.
 */

//namespace Components;

class ComponentsField {

  public $type,
         $format,
         $field,
         $fieldInfo;

  public function __construct($type, $format, $field, $fieldInfo) {
    $this->type = $type;
    $this->format = $format;
    $this->field = $field;
    $this->fieldInfo = $fieldInfo;
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
