<?php

/**
 * Section
 *
 * A KSS Comment Block that represents a single section containing a description,
 * modifiers, and a section reference.
 */

namespace Componentize;
use Scan\Kss\Section;

class SectionKSSnode extends Section {

  /**
   * Returns the reference number for the section.
   *
   * @param boolean $trimmed OPTIONAL
   *
   * @return string
   */
  public function getReference($trimmed = false) {
    if ($this->reference === null) {
      $referenceComment = $this->getReferenceComment();
      $referenceComment = preg_replace('/\.$/', '', $referenceComment);

      if (preg_match('/^\s*Style *guide:\s+(.*)/i', $referenceComment, $matches)) {
        $this->reference = trim($matches[1]);
      }
    }

    return ($trimmed && $this->reference !== null) ?
        static::trimReference($this->reference) :
        $this->reference;
  }

  /**
   * Gets the part of the KSS Comment Block that contains the section reference.
   *
   * @return string
   */
  protected function getReferenceComment() {
    $referenceComment = null;
    $commentSections = $this->getCommentSections();
    $lastLine = end($commentSections);

    if (preg_match('/^\s*Style *guide: \w/i', $lastLine) ||
        preg_match('/^\s*No styleguide reference/i', $lastLine)) {
      $referenceComment = $lastLine;
    }

    return $referenceComment;
  }

  /**
   * Returns the source file path where the comment block was located.
   *
   * @return string
   */
  public function getFilePath() {
    if ($this->file === null) {
      return '';
    }
    return dirname($this->file->getPathname());
  }
}
