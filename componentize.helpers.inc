<?php

/**
 * Helper function to flatten JSON component variables.
 *
 * @param $vars
 *  A JSON data structure as an associative array.
 * @param string $parent
 *  The name of the parent.
 * @return array|null
 *  Our flattened data structure.
 */
function _componentize_flatten_variables($vars, $parent = '') {
  $result = array();

  if (!is_array($vars)) {
    return NULL;
  }

  forEach ($vars as $name => $item) {
    $key = $parent . $name;

    if (is_array($item)) {
      // Is JSON object or JSON array?
      $is_json_object = (bool) count(array_filter(array_keys($item), 'is_string'));

      if ($is_json_object) {
        $result[$key . ' (object{})'] = _componentize_flatten_variables($item, $key . '.') ?: $name . ' (object{})';
      }
      else {
        $result[$key . ' (array[])'] = _componentize_flatten_variables($item[0], $key . '.[ ].') ?: $name . ' (array[])';
        continue;
      }
    }
    else {
      $result[$key] = $name;
    }
  }

  return $result;
}


/**
 * Helper function to restore flat data structure.
 *
 * @param $vars
 *  An array of component variables.
 * @param $keys
 *  An array of flat component variables for duplicate checking.
 * @param string $var
 *  The flattened template variable.
 *
 * @return array $result
 *  An array of unique keys.
 */
function _componentize_unflatten_variable($vars, $keys, $var) {
  // Unflatten our template variable to prepare for rendering.
  $temp = &$vars;
  $result = array();
  $parts = explode('.', $var);
  $count = count($parts);
  $i = 0;

  foreach ($parts as $part_name) {
    if (++$i === $count) {
      $result[] = $part_name;
      return $result;
    }

    if ($part_name === '[ ]') {
      //@todo Figure out a way to properly handle multiple nested arrays.
      if (!empty($temp)) {
        foreach ($temp as $key => $item) {
          $var = preg_replace('/\[ \]/', '[' . $key . ']', $var, 1);
          if (!array_key_exists($var, $keys)) {
            $result[] = $key;
            $temp = &$temp[$key];
            continue 2;
          }
        }
      }
      $result[] = 0;
      $temp = &$temp[];
    }
    else {
      $result[] = $part_name;
      $temp = &$temp[$part_name];
    }
  }
}

/**
 * Convert a given string value to an array, by splitting on comma and
 * whitespace. No empty values will be returned.
 *
 * @param string $value
 *
 * @return array
 */
function componentize_to_array($value) {
  return preg_split('/[\s,]+/', $value, NULL, PREG_SPLIT_NO_EMPTY);
}
