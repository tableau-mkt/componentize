<?php
/**
 * Override the Parser methods to allow KSS-node syntax.
 */

namespace Componentize;
use Scan\Kss\Parser;

class ParserKSSnode extends Parser {
  /**
   * Checks to see if a comment block is a KSS Comment block
   *
   * @param string $comment
   *
   * @return boolean
   */
  public static function isKssBlock($comment)
  {
      $commentLines = explode("\n\n", $comment);
      $lastLine = end($commentLines);
      return preg_match('/^\s*Style guide: \w/i', $lastLine) ||
          preg_match('/^\s*No style guide reference/i', $lastLine);
  }
}
