<?php
/**
 * @file Field handler: number
 */

namespace Components;

class ComponentsFieldNumber extends ComponentsField {

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
    );
  }

}
