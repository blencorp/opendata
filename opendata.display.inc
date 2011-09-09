<?php

/** 
 * Creates a view display
 * 
 * @param $form_state 
 *    An array holding the form data 
 * 
 * @return 
 *    A $form array with user provided values.
 *
 */
function opendata_create_display(&$view, $type, &$form_state) {
  
  $data_table_id = $form_state['storage']['general_values']['details']['data_table'];
  $data_table_name = _opendata_get_tablename($data_table_id);
  $office = _opendata_formatter($form_state['storage']['general_values']['details']['office'], 'lower');
  $fiscal_year = $form_state['storage']['general_values']['details']['fiscal_year'];
  $full_title =  $form_state['storage']['general_values']['details']['title'];
  $title =  _opendata_formatter($form_state['storage']['general_values']['details']['title'], 'hyphen');
  $description = _opendata_formatter($form_state['storage']['general_values']['details']['description'], 'html');

  $unique_name = substr('od_' . $office . '_' . $fiscal_year . '_' . _opendata_formatter($form_state['storage']['general_values']['details']['title'], 'underscore'), 0, 30);

  $url = 'grants/' . $office . '/' . $fiscal_year . '/' . $title;

  $page_limit = $form_state['storage']['summary_values']['page_limit'];
  $table_key = $form_state['storage']['summary_values']['table_key'];
  $map_title = $form_state['storage']['summary_values']['summary_map']['map_title'];
  $map_sub_title = $form_state['storage']['summary_values']['summary_map']['map_sub_title'];
  $map_lat = $form_state['storage']['summary_values']['summary_map']['map_lat'];
  $map_long = $form_state['storage']['summary_values']['summary_map']['map_long'];

  // get the list of column names
  $colnames = array();
  $colnames = _opendata_get_fields($data_table_id);

  // save relevant field names for the map data
  $other_lat = $colnames[$map_lat];
  $other_lon = $colnames[$map_long];
  $name_field = $colnames[$map_title];
  $description_field = $colnames[$map_sub_title];

  // get the name of the selected table key field
  $table_key = $colnames[$table_key];

  // set the detail perm url based on the selected table key field
  $detail_url = $url . '/detail/[' . $table_key . ']?' . $table_key . '=[' . $table_key . ']';
  
  //Get array of the summary fields
  $summary_fields = array();
  $summary_fields = $form_state['storage']['summary_values']['summary_fields'];

  // get an array of the detail page fields
  $detail_fields = array();
  $detail_fields = $form_state['storage']['published_values']['detail_fields'];

  // get selected fields
  $selected_fields = array();
  $selected_fields = _opendata_build_view_summary_fields($data_table_name, $colnames, $summary_fields, $detail_url, $table_key);

  // get selected fields
  $filter_fields = array();
  $filter_fields = _opendata_build_view_filter_fields($data_table_name, $colnames, $summary_fields, $detail_url, $table_key);

  if ($type == 'default') {
    $handler = $view->new_display('default', $full_title . ' default', 'default');

    $handler->override_option('fields', $selected_fields);
	  $handler->override_option('filters', $filter_fields);

    $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'table wizard administration',
    ));
  
    $handler->override_option('cache', array(
      'type' => 'none',
    ));
  
    $handler->override_option('header_format', '1');
    $handler->override_option('header_empty', 0);
    $handler->override_option('empty', 'There are no rows in this table.');
    $handler->override_option('empty_format', '1');
    $handler->override_option('items_per_page', $page_limit);
    $handler->override_option('use_pager', '1');
    $handler->override_option('style_plugin', 'table');
  
    // build the style options
    //TODO: work with _opendata_build_style_options
    $style_options = array();
    $style_options = _opendata_build_style_options($colnames, $summary_fields);
    $handler->override_option('style_options', $style_options);
    $handler->override_option('row_plugin', '');
    $handler->override_option('displays', array());
  } elseif ($type == 'desc_block') {
    $handler = $view->new_display('block', $title . ' description block', 'block_1');
    $handler->override_option('fields', array(
      'nothing' => array(
        'label' => '',
        'alter' => array(
          'text' => $description,
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'exclude' => 0,
        'id' => 'nothing',
        'table' => 'views',
        'field' => 'nothing',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('items_per_page', 1);
    $handler->override_option('use_pager', '0');
    $handler->override_option('style_plugin', 'default');
    $handler->override_option('style_options', NULL);
    $handler->override_option('row_plugin', 'fields');
    $handler->override_option('block_description', '');
    $handler->override_option('block_caching', -1);
  } elseif ($type == 'summary_page') {
    $url = 'grants/' . $office . '/' . $fiscal_year . '/' . $title;
    $handler = $view->new_display('page', $full_title . ' summary table listing', 'page_1');
    
    $handler->override_option('title', $full_title);
    $handler->override_option('path', $url); 
  } elseif ($type == 'openlayers_data') {
    $handler = $view->new_display('openlayers', $full_title . ' map data', 'openlayers_1');
    $map_fields = array();
    $a1 = $data_table_name; 
    $a2 = $other_lat;
    $a3 = $other_lon; 
    $a4 = $name_field; 
    $a5 = $description_field;
    $a6 = $table_key;
    $a7 = $detail_url;
    $map_fields = _opendata_build_map_fields($a1, $a2, $a3, $a4, $a5, $a6, $a7);
    $handler->override_option('fields', $map_fields);
    $handler->override_option('items_per_page', 0);
    $handler->override_option('style_plugin', 'openlayers_data');
    $handler->override_option('style_options', array(
      'grouping' => '',
      'data_source' => array(
        'value' => array('other_latlon' => 'other_latlon',),
        'other_lat' => $other_lat,
        'other_lon' => $other_lon, 
        'openlayers_wkt' => $table_key,
        'other_top' => $table_key,
        'other_right' => $table_key,
        'other_bottom' => $table_key,
        'other_left' => $table_key,
        'name_field' => $name_field,
        'description_field' => $description_field,
      ),
    ));
  } elseif ($type == 'map_block') {
    $handler = $view->new_display('block', $full_title . ' map block', 'block_2');
    $handler->override_option('items_per_page', 0);
    $handler->override_option('use_pager', '0');
    $handler->override_option('style_plugin', 'openlayers_map');
    $handler->override_option('style_options', array(
     'preset' => $unique_name,
    ));
    $handler->override_option('block_description', '');
    $handler->override_option('block_caching', -1);
  } elseif ($type == 'csv_block') {
    $handler = $view->new_display('feed', $full_title . ' csv block', 'feed_1');
    $handler->override_option(
        'items_per_page', 0
    );
    $handler->override_option('style_plugin', 'views_csv');
    $handler->override_option('style_options', array(
      'mission_description' => FALSE,
      'description' => '',
      'attach_text' => 'CSV',
      'provide_file' => 1,
      'filename' => $unique_name . '.csv',
      'parent_sort' => 0,
      'seperator' => ',',
      'quote' => 1,
      'header' => 1,
      )
    );
    $handler->override_option('row_plugin', '');
    $handler->override_option('path', $url . '/csv');
    
    $handler->override_option('displays', array(
      'page_1' => 'page_1',
      'default' => 0,
      'block_1' => 0,
    ));
  } elseif ($type == 'detail_page') {
    $handler = $view->new_display('page', $full_title . ' detail', 'page_2');
    
    // build detail page fields
    $selected_fields = array();
    $a1 = $data_table_name;
    $a2 = $colnames;
    $a3 = $detail_fields;
    $a4 = $table_key;
    $selected_fields = _opendata_build_detail_view_fields($a1, $a2, $a3, $a4);
    $handler->override_option('fields', $selected_fields);
    $handler->override_option('arguments', array(
      $table_key => array(
        'id' => $table_key,
        'table' => $data_table_name,
        'field' => $table_key,
        'label' => _opendata_formatter($table_key, 'space'),
        'exclude' => 0,
        'relationship' => 'none',
        'separator' => '',
        'exclude' => 1,
      ),
    ));
    $handler->override_option('title', $full_title);
    $handler->override_option('path', $url . '/detail/%');
    $handler->override_option('style_plugin', 'default');
    $handler->override_option('style_options', NULL);
    $handler->override_option('row_plugin', 'fields');
  } 
  return $form;
}
