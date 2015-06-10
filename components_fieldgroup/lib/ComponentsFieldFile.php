<?php
/**
 * @file Field handler: file
 */

namespace Components;

class ComponentsFieldFile extends ComponentsField {

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
      'uri' => $item['uri'],
      'description' => $item['description']
    );
  }

}
