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

    'componentize_partials' => array(
      '#type' => 'textfield',
      '#title' => 'Partials directory',
      '#description' => t('Folder location of component partials. Default: !folder',
        array('!folder' => '<code>' . COMPONENTIZE_PARTIALS . '</code>')
      ),
      '#default_value' => variable_get('componentize_partials', COMPONENTIZE_PARTIALS),
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

    'componentize_supress_messages' => array(
      '#type' => 'checkbox',
      '#title' => 'Supress messages',
      '#description' => t('Avoid showing missing component file messages everywhere but componentize admin pages.'),
      '#default_value' => variable_get('componentize_supress_messages', FALSE),
    ),
  );

  $form['exclude'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exclusions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Get a list of all themes.
  $themes = list_themes();
  foreach ($themes as $name => $theme) {
    $theme_options[$name] = $theme->info['name'];
  }

  $form['exclude']['componentize_exclude_themes'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Exclude on specific themes',
    '#description' => t('Check which themes you do not want to load components on.'),
    '#options' => $theme_options,
    '#default_value' => variable_get('componentize_exclude_themes', array()),
  );

  $form['exclude']['componentize_exclude_paths'] = array(
    '#type' => 'textarea',
    '#title' => 'Exclude on URL Patterns',
    '#description' => t('A list of URL patterns you do not want to load components on. Separate with newline or whitespace. Supports wildcard using *.'),
    '#default_value' => variable_get('componentize_exclude_paths', ''),
  );

  $form['asset_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS/JS Group/Weight Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['asset_options']['componentize_js_group'] = array(
    '#type' => 'numberfield',
    '#title' => t('JS Group'),
    '#description' => t('JS group weight value. Defaults to 100 (JS_THEME).'),
    '#default_value' => variable_get('componentize_js_group', JS_THEME),
  );

  $form['asset_options']['componentize_js_weight'] = array(
    '#type' => 'numberfield',
    '#title' => t('JS Weight'),
    '#description' => t('JS weight within group value.'),
    '#default_value' => variable_get('componentize_js_weight', 0),
  );

  $form['asset_options']['componentize_js_every_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('JS on every page'),
    '#description' => t('Bundle/aggregate on every page.'),
    '#default_value' => variable_get('componentize_js_every_page', TRUE),
  );

  $form['asset_options']['componentize_css_group'] = array(
    '#type' => 'numberfield',
    '#title' => t('CSS Group'),
    '#description' => t('CSS group weight value. Defaults to 100 (CSS_THEME).'),
    '#default_value' => variable_get('componentize_css_group', CSS_THEME),
  );

  $form['asset_options']['componentize_css_weight'] = array(
    '#type' => 'numberfield',
    '#title' => t('CSS Weight'),
    '#description' => t('CSS weight within group value.'),
    '#default_value' => variable_get('componentize_css_weight', 0),
  );

  $form['asset_options']['componentize_css_every_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('CSS on every page'),
    '#description' => t('Bundle/aggregate on every page.'),
    '#default_value' => variable_get('componentize_css_every_page', TRUE),
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
