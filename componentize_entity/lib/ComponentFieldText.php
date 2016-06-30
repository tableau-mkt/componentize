<?php
/**
 * @file Field handler: text.
 */

namespace Componentize;

class ComponentFieldText extends ComponentField {

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
    return isset($item['safe_value']) ? $item['safe_value'] : '';
  }

}
