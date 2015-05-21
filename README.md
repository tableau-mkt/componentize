# Components API

Drupal module -- directly use style-guide web components (KSS) via Handlebars templates as fieldgroup displays.

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
  * [KSS-PHP](https://github.com/scaninc/kss-php), a KSS component parser for PHP.
  * [Lightncandy](https://github.com/zordius/lightncandy), Handlebars integration.
1. Create a `/sites/all/components` folder.
1. Drop in a few [KSS style-guide](http://warpspire.com/kss) web components.
1. Install Drupal module via Drush or admin UI.

### Drupal Config
1. Generate component entities.
1. Set content-type fieldgroup display mode to Component.
1. Add fields, chose mapping for template variables to field names.
1. View node.

### Developer Use
This module allows you to refer to components and render content through Handlebars templates within PHP code inside Drupal.  Here's an example...

```php
function my_module_block_view() {
  $component = new Component('box.badge');
  $component->render(array(
    'title' => 'RoadShow',
    'score' => '56',
  ));
}
```
