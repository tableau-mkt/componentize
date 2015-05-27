<?php
/**
 * @file Admin settings for components.
 */

/**
 * Settings admin configs.
 */
function components_settings() {
  $form = array(
    '#prefix' => t('Saving this form will re-scan and regenerate stored component records.'),

    'components_directory' => array(
      '#type' => 'textfield',
      '#title' => 'Directory',
      '#description' => t('Folder location of site components. Default: !folder',
        array('!folder' => COMPONENTS_DIRECTORY
      )),
      '#default_value' => variable_get('components_directory', COMPONENTS_DIRECTORY),
    ),

    'components_storage' => array(
      '#type' => 'checkbox',
      '#options' => drupal_map_assoc(array(
        0 => t('Full'),
        1 => t('Less'),
        2 => t('None'),
      )),
      '#title' => 'Config storage',
      '#description' => t('Avoid regenerating with less aggresive caching. Data will be stored in either: variables, cache or regenerated on every page load.'),
      '#default_value' => variable_get('components_storage', array()),
    ),
  );

  $form['#submit'][] = 'components_regenerate';

  // $form['components_blocks'] = array(
  //   '#type' => 'checkbox',
  //   '#options' => _component_list(TRUE),
  //   '#multiple' => TRUE,
  //   '#title' => 'Blocks',
  //   '#description' => t('Components to generate as blocks.'),
  //   '#default_value' => variable_get('components_blocks', array()),
  // );

  return system_settings_form($form);
}


/**
 * Regenerate registered component variables.
 */
function components_regenerate() {
  // Second param is reset.
  _components_list(FALSE, TRUE);
}
