<?php
/**
 * @file Admin settings for components.
 */

/**
 * Settings admin configs.
 */
function componentize_settings() {
  $form = array(
    '#prefix' => t('Saving this form will re-scan and regenerate stored component records.'),

    'componentize_directory' => array(
      '#type' => 'textfield',
      '#title' => 'Directory',
      '#description' => t('Folder location of site components (default: !folder).',
        array('!folder' => COMPONENTIZE_DIRECTORY
      )),
      '#default_value' => variable_get('componentize_directory', COMPONENTIZE_DIRECTORY),
    ),

    'componentize_assets' => array(
      '#type' => 'textfield',
      '#title' => 'Compiled assets directory',
      '#description' => t('Folder location of compiled site components CSS and javascript (default: !folder).', array(
          '!folder' => COMPONENTIZE_COMPILED_ASSETS)
        ) . '<br>' . t('Filenames should be: !css and !js', array(
          '!css' => '<code>' . COMPONENTIZE_COMPILED_CSS . '</code>',
          '!js' => '<code>' . COMPONENTIZE_COMPILED_JS . '</code>'
      )),
      '#default_value' => variable_get('componentize_assets', COMPONENTIZE_COMPILED_ASSETS),
    ),

    'componentize_storage' => array(
      '#type' => 'select',
      '#options' => array(
        'variable' => t('Strong - Variable'),
        'cache' => t('Less - Cache'),
        'none' => t('None - Files'),
      ),
      '#title' => 'Config storage',
      '#description' => t('Avoid regenerating with less aggresive caching. Data will be stored in either: variables, cache or regenerated on every page load.'),
      '#default_value' => variable_get('componentize_storage', 1),
    ),
  );

  $form['#submit'][] = 'componentize_regenerate';

  // $form['componentize_blocks'] = array(
  //   '#type' => 'checkbox',
  //   '#options' => componentize_list(TRUE),
  //   '#multiple' => TRUE,
  //   '#title' => 'Blocks',
  //   '#description' => t('Components to generate as blocks.'),
  //   '#default_value' => variable_get('componentize_blocks', array()),
  // );

  return system_settings_form($form);
}


/**
 * Regenerate registered component variables.
 */
function componentize_regenerate() {
  // Second param is reset.
  componentize_list(FALSE, TRUE);
}
