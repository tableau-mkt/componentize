<?php
/**
 * @file Browse available component structure.
 */

/**
 * Menu callback for viewing available components.
 */
function componentize_library_output() {
  $table = array(
    'attributes' => array(),
    'colgroups' => array(),
    'sticky' => TRUE,
    'empty' => t('No components added yet.'),
    'header' => array(array(
        'data' => t('Section'),
        'field' => 'section',
      ), array(
        'data' => t('Component'),
        'field' => 'component',
      ), array(
        'data' => t('Variables'),
        'field' => 'variables',
      ), array(
        'data' => t('Modifiers'),
        'field' => 'modifiers',
      ),
  ));

  // List all components.
  $table['rows'] = array();
  foreach (componentize_list() as $component) {
    $table['rows'][] = array(array(
        'data' => $component->getSection(),
        'field' => 'section',
      ), array(
        'data' => $component->getTitle(),
        'field' => 'component',
      ), array(
        'data' => theme_item_list(array(
          'items' => $component->getVariables(),
          'type' => 'ol',
          'attributes' => array(),
          'title' => NULL,
        )),
        'field' => 'variables',
      ), array(
        'data' => theme_item_list(array(
          'items' => $component->getModifiers(),
          'type' => 'ol',
          'attributes' => array(),
          'title' => NULL,
        )),
        'field' => 'modifiers',
      ),
    );
  }
  $table['caption'] = count($table['rows']) . ' components found';

  return theme_table($table);
}
