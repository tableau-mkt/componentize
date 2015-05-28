<?php
/**
 * @file API documentation for Components.
 */


/**
 * Add custom components.
 *
 * @param array $components
 *
 *
 * @see hook_admin_menu_output_build()
 */
function hook_components_list_alter(&$components_list, $shallow) {
  if ($shallow) {
    return $components[] = 'my_component';
  }

  $template = <<<EOM
<span class="my-component {{ modifier }}">
  <strong class="my-component__title">{{ title }}</strong>
  <p class="my-component__body">{{ body }}</p>
</span>
EOM;

  $components['my_component'] = array(
    'template' => $template,
    'renderer' => $handlebar->compile($template),
    'variables' => array(
      'title',
      'body',
    ),
    'modifiers' => array(
      'my_component--dark',
      'my_component--wacky',
    ),
  );
}


/**
 * Use a component to render code.
 */
function my_module_block_view() {
  $component = new Component('box.badge');
  $component->render(array(
    'label' => 'Smarty Pants',
    'score' => '99',
  ));
}
