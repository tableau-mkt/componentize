# Componentize!

**Integrate/import your well-structured component style guide into Drupal.**

Prefer to organize your styles into components?  Want to use [Handlebars](http://handlebarsjs.com) as templates?  Want to use an automated, living style guide ([KSS](http://warpspire.com/kss/styleguides)) that's synced to Drupal?  Now you can!

Keep a clean front-end structure and make Drupal aware with convenient developer APIs; render CMS content through Handlebars templates!  Map entities directly into components without writing a line of code using fields and [fieldgroups](https://www.drupal.org/project/field_group).

## Get Started

### Basic Install
1. Install [Composer](https://getcomposer.org/doc/00-intro.md) to manage dependencies.
1. Ensure you have [Composer Manager](https://www.drupal.org/project/composer_manager).  Some helpful [notes](https://www.drupal.org/node/2405805).
1. Tell composer where to manage vendor code with the these two variables.  Add to your site's settings.php file or use `drush vset`.
  * `$conf['composer_manager_vendor_dir'] = 'vendor';`
  * `$conf['composer_manager_file_dir'] = './';`
1. Build your site root composer config: `drush composer-json-rebuild`
1. Get library dependencies via `composer install --prefer-dist`
  * [KSS-PHP](https://github.com/scaninc/kss-php), KSS component parser for PHP
  * [Lightncandy](https://github.com/zordius/lightncandy), Handlebars rendering
1. Get Drupal dependencies (for sub-modules).
  * [Chaos tool suite](http://www.drupal.org/project/ctools)
  * [Field Group](http://www.drupal.org/project/field_group)
  * [Field formatter settings](http://www.drupal.org/project/field_formatter_settings)
  * [Entity view modes](https://www.drupal.org/project/entity_view_mode)
1. Create a `/sites/all/components` folder, and drop in a few components, see examples.
1. Install Drupal module (and any sub-modules) via Drush or admin UI.
1. Visit the admin page (`admin/structure/componentize`), alter include paths if desired and choose cache aggresiveness for tracking component data.  Save settings once to generate components from your style guide components if you are using caching.

### [Development workflow](https://github.com/tableau-mkt/componentize/wiki/Development%20Workflow)

### Easy [admin use](https://github.com/tableau-mkt/componentize/wiki/Admin-Use)

### Check out the [component APIs](https://github.com/tableau-mkt/componentize/wiki/Component-APIs)
