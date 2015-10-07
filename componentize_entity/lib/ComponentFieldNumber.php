<?php
/**
 * @file Field handler: number
 */

namespace Componentize;

class ComponentFieldNumber extends ComponentField {

  /**
   * Variables from field value(s): number.
   *
   * @param array $item
   *   Field value array.
   *
   * @return array
   *   Variable data to send to template.
   */
  public function getValues($item) {
    return array(
      'prefix' => $item['prefix'],
      'value' => $item['value'],
    );
  }

}
