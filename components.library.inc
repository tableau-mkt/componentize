<?php
/**
 * @file Browse available component structure.
 */

/**
 * Menu callback for viewing available components.
 */
function _components_library_output() {
  $header = array(
    t('Component'),
    t('Section'),
    t('Modifiers'),
    t('Variables'),
  );

  // List all components.
  $rows = array(
  if (!empty(_components_list())) {
    foreach (_components_list() as $component) {


      $rows[] = array(array(
          'data' => $component->getName(),
          'class' => 'component',
        ), array(
          'data' => $component->getSection(),
          'class' => 'section'
        ), array(
          'data' => $component->getModifiers(),
          'class' => 'modifiers'
        ), array(
          'data' => $component->getVariables(),
          'class' => 'variables'
        ),
      );


      // Row with attributes on the row and some of its cells.
      array(
        'data' => array('Cell 1', array('data' => 'Cell 2', 'colspan' => 2)), 'class' => array('funky')
      )
    );


      $output[] = $component->getName();
    }
  }
  else {
    $empty = $variables ['empty'];
  }
$header = $variables ['header'];
  $rows = $variables ['rows'];
  $attributes = $variables ['attributes'];
  $caption = $variables ['caption'];
  $colgroups = $variables ['colgroups'];
  $sticky = $variables ['sticky'];
  $empty = $variables ['empty'];


  $table = array(
    'header' => $header,
    'rows' => $rows,
    'sticky' => TRUE
    'empty' => t('No components added yet.'),
  );

  return drupal_theme('table', $table);
}
