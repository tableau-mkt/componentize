<?php
/**
 * @file Context reaction for component alteration.
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
      '#options' => drupal_map_assoc(componentize_list(TRUE)),
      '#default_value' => $values['component'],
      '#weight' => 1,
    );
    $form['modifier'] = array(
      '#title' => t('Modifier'),
      '#type' => 'textfield',
      '#default_value' => $values['modifier'],
      '#maxlength' => 256,
      '#size' => 20,
      '#weight' => 2,
    );

    return $form;
  }


  /**
   * {@inheritdoc}
   */
  public function execute(&$component, &$data) {
    // Work through all contexts.
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        // Relavant component check.
        $options = $context->reactions[$this->plugin];
        if ($options['component'] === $component->getName()) {
          $this->alter($options, $component, $data);
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
  private function alter($options, &$component, &$data) {
    if (isset($options['modifier'])) {
      $component->setModifier($options['modifier']);
    }
  }
}
