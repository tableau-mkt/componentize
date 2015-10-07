<?php
/**
 * @file Field handler: image
 */

namespace Componentize;

class ComponentFieldImage extends ComponentFieldFile {

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
      'alt' => $item['alt'],
      'title' => $item['title'],
      'width' => $item['width'],
      'height' => $item['height'],
    );
  }

}
