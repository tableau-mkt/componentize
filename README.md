# Styleguide Components API

**Bring the structure of KSS into Drupal objects.**

Want to organize your styles into components? Want to use [Handlebars](http://handlebarsjs.com) as templates? Want an automated living styleguide via [KSS](http://warpspire.com/kss/styleguides)? Use a clean front-end structure and make Drupal aware with convenient developer APIs; render CMS content through Handlebars templates! Even map eventity [fieldgroups](https://www.drupal.org/project/field_group) directly to components without writing a line of code.

## Get Started

### Basic Install
1. Install [Composer](https://getcomposer.org/doc/00-intro.md) to manage dependencies.
1. Ensure you have [Composer Manager](https://www.drupal.org/project/composer_manager). Some helpful [notes](https://www.drupal.org/node/2405805).
1. Tell composer where to manage vendor code with the follow two variables. Add to your site's settings.php file or use `drush vset`...
```php
$conf['composer_manager_vendor_dir'] = 'vendor';
$conf['composer_manager_file_dir'] = './';
```
1. Build your project root composer config: `drush composer-json-rebuild`
1. Get dependencies via `drush composer install --prefer-dist`
  * [KSS-PHP](https://github.com/scaninc/kss-php), KSS component parser for PHP
  * [Lightncandy](https://github.com/zordius/lightncandy), Handlebars rendering
1. Create a `/sites/all/components` folder.
1. Drop in a few components, see examples.
1. Install Drupal module via Drush or admin UI.

### Drupal Config
1. Visit the admin page for basic settings. Choose cache aggresiveness for tracking component data. Recommenadation: Off.
1. Set a content-type fieldgroup Component within display mode settings.
1. Add fields, chose mapping for template variables to field names.
1. View node.

### Developer Use
This module allows you to refer to components and render content through Handlebars templates within PHP code inside Drupal.  Here's an example...

```php
function my_module_block_view() {
  // Refer to an existing component.
  $component = new Component('forms.textfield');
  $component->setModifier('');
  // Render data from the CMS through handlebars.
  $component->render(array(
    'label' => 'First Name',
    'name' => 'f_name',
    'placeholder' => 'John Doe',
  ));
}

```html
<div class="textfield {{modifier_class}}">
  <input class="textfield__input" name="{{}}">
</div>
```
