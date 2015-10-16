<?php
/**
 * @file Admin settings for components.
 */

/**
 * Settings admin configs.
 */
function componentize_settings() {
  $css_default = COMPONENTIZE_COMPILED_ASSETS . '/' . COMPONENTIZE_COMPILED_CSS;
  $js_default = COMPONENTIZE_COMPILED_ASSETS . '/' . COMPONENTIZE_COMPILED_JS;

  $form = array(
    '#prefix' => t('Saving this form will re-scan and regenerate stored component records.'),

    'componentize_directory' => array(
      '#type' => 'textfield',
      '#title' => 'Directory',
      '#description' => t('Folder location of site components. Default: !folder',
        array('!folder' => '<code>' . COMPONENTIZE_DIRECTORY . '</code>')
      ),
      '#default_value' => variable_get('componentize_directory', COMPONENTIZE_DIRECTORY),
    ),

    'componentize_css' => array(
      '#type' => 'textfield',
      '#title' => 'Compiled CSS',
      '#description' => t('Location of compiled CSS. Default: !filepath', array(
        '!filepath' => '<code>' . $css_default . '</code>'
      )),
      '#default_value' => variable_get('componentize_css', $css_default),
    ),

    'componentize_js' => array(
      '#type' => 'textfield',
      '#title' => 'Compiled JS',
      '#description' => t('Location of compiled JS. Default: !filepath', array(
        '!filepath' => '<code>' . $js_default . '</code>'
      )),
      '#default_value' => variable_get('componentize_js', $js_default),
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
      '#default_value' => variable_get('componentize_storage', 'none'),
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
