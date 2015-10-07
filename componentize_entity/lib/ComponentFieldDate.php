<?php
/**
 * @file Field handler: date
 */

namespace Componentize;

class ComponentFieldDate extends ComponentField {

  /**
   * Variables from field value(s): file.
   *
   * @param array $item
   *   Field value array.
   *
   * @return array
   *   Variable data to send to template.
   */
  public function getValues($item) {
    return array(
      'timezone' => $item['timezone'],
      'value' => $item['value'],
    );
  }

}
