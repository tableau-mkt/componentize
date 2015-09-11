<?php
/**
 * Override the Parser methods to allow KSS-node syntax.
 */

namespace Componentize;
use Componentize\Section;
use Scan\Kss\Parser;
use Scan\Kss\CommentParser;
use Symfony\Component\Finder\Finder;

class ParserKSSnode extends Parser {

  /**
   * Override KSS comments parser to improve matching.
   *
   * @param string|array $paths
   *  A string or array of the paths to scan for KSS comments.
   */
  public function __construct($paths) {
    $finder = new Finder();
    // Only accept css, sass, scss, less, and stylus files.
    $finder->files()->name('/\.(css|sass|scss|less|styl(?:us)?)$/')->in($paths);

    foreach ($finder as $fileInfo) {
      $file = new \splFileObject($fileInfo);
      $commentParser = new CommentParser($file);

      foreach ($commentParser->getBlocks() as $commentBlock) {
        if (static::isKssBlock($commentBlock)) {
          $this->addSection($commentBlock, $file);
        }
      }
    }
  }

  /**
   * Adds a section to the Sections collection
   *
   * @param string $comment
   * @param \splFileObject $file
   */
  protected function addSection($comment, \splFileObject $file) {
      $section = new SectionKSSnode($comment, $file);
      $this->sections[$section->getReference(true)] = $section;
      $this->sectionsSortedByReference = false;
  }

  /**
   * Checks to see if a comment block is a KSS Comment block
   *
   * @param string $comment
   *
   * @return boolean
   */
  public static function isKssBlock($comment) {
    $commentLines = explode("\n\n", $comment);
    $lastLine = end($commentLines);

    return preg_match('/^\s*Style *guide: \w/i', $lastLine) ||
        preg_match('/^\s*No style guide reference/i', $lastLine);
  }

}
