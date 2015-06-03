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
      '#title' => 'Component directory',
      '#description' => t('Folder location of site components (default: !folder).',
        array('!folder' => COMPONENTS_DIRECTORY
      )),
      '#default_value' => variable_get('components_directory', COMPONENTS_DIRECTORY),
    ),

    'components_assets' => array(
      '#type' => 'textfield',
      '#title' => 'Compiled assets directory',
      '#description' => t('Folder location of compiled site components CSS and javascript (default: !folder). Files should be named: style.css and script.js',
        array('!folder' => COMPONENTS_COMPILED_ASSETS
      )),
      '#default_value' => variable_get('components_assets', COMPONENTS_COMPILED_ASSETS),
    ),

    'components_storage' => array(
      '#type' => 'select',
      '#options' => array(
        'variable' => t('Strong - Variable'),
        'cache' => t('Less - Cache'),
        'none' => t('None - Files'),
      ),
      '#title' => 'Config storage',
      '#description' => t('Avoid regenerating with less aggresive caching. Data will be stored in either: variables, cache or regenerated on every page load.'),
      '#default_value' => variable_get('components_storage', 1),
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
