<?php
/**
 * @file Context reaction class for component modifiers.
 */


class context_reaction_component_alter extends context_reaction {

  /**
   * Provide an admin form.
   */
  public function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array();

    $form['component'] = array(
      '#title' => t('Component'),
      '#type' => 'select',
      '#options' => drupal_map_assoc(components_list()),
      '#default_value' => $values['elements']['component'],
      '#weight' => 1,
    );
    $form['modifier'] = array(
      '#title' => t('Modifier'),
      '#type' => 'text',
      '#default_value' => $values['elements']['modifier'],
      '#size' => 20,
      '#weight' => 2,
    );

    return $form;
  }


  /**
   * {@inheritdoc}
   */
  public function execute(&$component) {
    // Work through all contexts.
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        // Relavant component check.
        $options = $context->reactions[$this->plugin];
        if ($options['component'] === $component->getName) {
          $this->alter($options, $component);
        }
      }
    }
  }


  /**
   * Use Context options to alter component rendering.
   *
   * @todo Allow open-ended, translatable variable overrides.
   *
   * @param array $options
   *   Content admin form options.
   * @param Component $component
   *   Component object about to be rendered.
   */
  private function alter($options, &$component) {
    if (isset($options['modifier'])) {
      $component->setModifier($options['modifier']);
    }
  }
}
