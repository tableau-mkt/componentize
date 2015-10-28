<?php
/**
 * @file Field handler: file.
 */

namespace Componentize;

class ComponentFieldFile extends ComponentField {

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
    return $this->collectProperties($item, array(
      'uri',
      'filename',
      'filesize',
      'mimetype',
      'description'
    )) + array(
      'url' => file_create_url($item['uri']),
    );
  }
}
