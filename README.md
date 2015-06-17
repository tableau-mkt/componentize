# Componentize!

**Integrate your well-structured style guide with Drupal.**

Prefer to organzie your styles into components?  Want to use [Handlebars](http://handlebarsjs.com) as templates?  Want to use an automated, living style guide ([KSS](http://warpspire.com/kss/styleguides)) and still work with Drupal?  Now you can!

Keep a clean front-end structure and make Drupal aware with convenient developer APIs; render CMS content through Handlebars templates!  Even map entity [fieldgroups](https://www.drupal.org/project/field_group) directly to components without writing a line of code ...and control components via Context.

## Get Started

### Basic Install
1. Install [Composer](https://getcomposer.org/doc/00-intro.md) to manage dependencies.
1. Ensure you have [Composer Manager](https://www.drupal.org/project/composer_manager). Some helpful [notes](https://www.drupal.org/node/2405805).
1. Tell composer where to manage vendor code with the these two variables. Add to your site's settings.php file or use `drush vset`.
  * `$conf['composer_manager_vendor_dir'] = 'vendor';`
  * `$conf['composer_manager_file_dir'] = './';`
1. Build your site root composer config: `drush composer-json-rebuild`
1. Get library dependencies via `drush composer install --prefer-dist --prefer-stable`
  * [KSS-PHP](https://github.com/scaninc/kss-php), KSS component parser for PHP
  * [Lightncandy](https://github.com/zordius/lightncandy), Handlebars rendering
1. Get Drupal dependencies (for sub-modules).
  * [Chaos tool suite](http://www.drupal.org/project/ctools)
  * [Field Group](http://www.drupal.org/project/field_group)
  * [Field formatter settings](http://www.drupal.org/project/field_formatter_settings)
  * [Context](http://www.drupal.org/project/context)
  * [Entity view modes](https://www.drupal.org/project/entity_view_mode)
1. Create a `/sites/all/components` folder, and drop in a few components, see examples.
1. Install Drupal module (and any sub-modules) via Drush or admin UI.
1. Visit the admin page (`admin/structure/componentize`), alter include paths if desired and choose cache aggresiveness for tracking component data.  Save settings once to generate components from your style guide components if you are using caching.

### Fieldgroup Config
1. Edit a content-type field display settings. Add a fieldgroup (or set and existing one) as a component, chose the component and optionally set a modifier.
1. Add fields to the group and chose mapping to the template variables.
1. Save the display mode.
1. View your node in the same view mode you edited, fields should be piped through your Handlebars template!

### Context Config
1. Add a Context reaction "Component Alter".
1. Set the component.
1. Set the modifier and save.

### Entity View Modes Config (todo)
1. Add a custom view mode to an entity bundle, or use a core mode.
1. Choose a component within the view mode settings.
1. Chose field mapping to the template variables.

### Recommended Companions
1. [CCK Blocks](https://www.drupal.org/project/cck_blocks) -- Turn your fieldgroup into a block on the node page.
1. [Title field UI](https://www.drupal.org/project/title_field_ui) or [Title](https://www.drupal.org/project/title) -- control title via field admin.


## Workflow
You may want to keep your components in a separate repo from your site code and theme.  For convenience set the local development location of your component library to somewhere outside your Drupal site code...
```php
$conf['componentize_directory'] = './my_components';
$conf['componentize_assets'] = './my_components/dist';
```


### Developer Use
This module allows you to refer to components written in front-end code and render content (through Handlebars templates, via PHP code) inside Drupal.  Control components in the following ways...

1. Register new components to house within a module, rather than the general site folder.
1. Place rendered components on the page via: block view, menu callbacks, views rows, etc.
1. Alter an existing component based on code conditions, like it's modifier or variable values.
1. Add or alter variable data before it is rendered through the template (eg: strings for translation).

#### Please see: [componentize.api.php](https://github.com/tableau-mkt/componentize/blob/7.x-1.x/componentize.api.php)

One quick example...

```php
function my_module_block_view() {
  // Refer to an existing component.
  $component = new Component('Type.Thing');
  $component->setModifier(variable_get('my_module_thing_modifier', 'special-skin'));
  // Render data from the CMS through handlebars.
  $component->render(array(
    'name' => variable_get('my_module_thing_title', 'The Title'),,
    'button' => variable_get('my_module_button_text', 'Click Here'),
    'placeholder' => variable_get('my_module_placeholder', 'String'),
    'description' => variable_get('my_module_desc', 'Easy to use'),
  ));
}
```

```html
<div class="fancy-thing {{ modifier_class }}">
  <label>{{ name }}</label>
  <input type="text" class="fancy-thing__input" placeholder="{{ placeholder }}">
  <button class="fancy-thing__button" type="button">{{ button }}</button>
  <p class="fancy-thing__desc">{{{ description }}}</p>
</div>
```

```html
<div class="fancy-thing special-skin">
  <label>The Title</label>
  <input type="text" class="fancy-thing__input" placeholder="String">
  <button class="fancy-thing__button" type="button">Click Here</button>
  <p class="fancy-thing__desc">Easy to use</p>
</div>
```
