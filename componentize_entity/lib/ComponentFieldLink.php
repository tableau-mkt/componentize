<?php
/**
 * @file Field handler: link
 */

namespace Componentize;

class ComponentFieldLink extends ComponentField {

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
      'url' => $item['url'],
      'title' => $item['title'],
      'attributes' => $item['attributes'],
    );
  }

}
