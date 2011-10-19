<?php
/**
 * @file
 * OpenData Module Helper Functions
 */

function _opendata_build_csv_fields($data_table_name, $colnames, $summary_fields, $table_key) {
  // instantiate an array to hold the final fields structure 
	$csv_fields = array();

	// IMPORTANT!
	// set the table key as the first field
	// to pass the right value to the detail page
	$csv_fields[$table_key] = array(
	 'id' => $table_key,
	 'table' => $data_table_name,
	 'field' => $table_key,
	 'label' => _opendata_formatter($table_key, 'space'),
	 'relationship' => 'none',
	 'separator' => '',
	 'exclude' => 1,
	);
	$csv_fields[$table_key]['display_order'] = 0;

	// find out the table key column name
	$table_key_column = $colnames[$table_key];

	// a temporary display order field to hold, if the
	// user does not enter any display order
	$temp_display_order = 999;

	foreach ($colnames as $colname) {    
		// Add the field_ prefix to the column name
		$selected_colname = 'field_selected_' . $colname;
		$exportable_colname = 'field_exportable_' . $colname;
		$sort_colname = 'field_display_order_' . $colname;   

		// If the field is selected, build the view field
		if ($summary_fields[$selected_colname] == 1 || $summary_fields[$exportable_colname] == 1) {
			$csv_fields[$colname] = array(
				'id' => $colname,
				'table' => $data_table_name,
				'field' => $colname,
				'label' => _opendata_formatter($colname, 'space'),
				'exclude' => 0,
				'relationship' => 'none',
				'alter' => array(
					'make_link' => 1,
					'path' => $url,
				),
			);
			 
			if ($summary_fields[$sort_colname] != '' or $summary_fields[$sort_colname] > 0) {
				$csv_fields[$colname]['display_order'] = $summary_fields[$sort_colname];
			} else {
				$csv_fields[$colname]['display_order'] = $temp_display_order;
				$temp_display_order++;
			}
		}
	}

	// sort the display order
	$csv_fields = _opendata_sort_display_order($csv_fields, 1);

	return $csv_fields;
}

// this function used to get the title field that is selected from 
// the detail page creation page's title radio button. 
// it has since been deprecated and replaced by a drop down list
function _opendata_get_title_field($colnames, $fields) {
	$tfield = 'None';

	// instantiate a flag to see if a title field is selected
	$is_titled = false;

	foreach ($colnames as $colname) {    
		$title_colname = 'field_detail_title_' . $colname;

		//if ($fields[$title_colname] == '1') {
		if ($fields[$title_colname] == '0') {
			$tfield = $colname;
			$is_titled = true;
		}
	}

	// if a title field is not defined, set the first selected field as title 
	// field
	if ($is_titled == false) {
		$counter = 0;
		foreach ($colnames as $colname) {    
			if ($counter > 2) {
				$tfield = $colname;
				break;
			}
			$counter++;
		}
	}

	//dbm($colnames);

	return $tfield;
}

function _opendata_build_map_fields($data_table_name, $lat, $lon, $title, $subtitle, $table_key, $detail_url, $detail_title) {
  // instantiate an array to hold the final fields structure 
  $map_fields = array();

  $map_fields[$table_key . '_arg'] = array(
    'id' => $table_key . '_arg',
    'table' => $data_table_name,
    'field' => $table_key,
    'label' => $table_key . '_arg',
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

	$map_fields[$detail_title . '_arg'] = array(
		'id' => $detail_title . '_arg',
		'table' => $data_table_name,
		'field' => $detail_title,
		'label' => $detail_title . '_arg',
		'relationship' => 'none',
    'separator' => '',
		'exclude' => 1,
	);
	//$map_fields[$detail_title . '_arg']['display_order'] = 1;
	
  $map_fields[$lat] = array(
    'id' => $lat,
    'table' => $data_table_name,
    'field' => $lat,
    'label' => _opendata_formatter($lat, 'space'),
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

  $map_fields[$lon] = array(
    'id' => $lon,
    'table' => $data_table_name,
    'field' => $lon,
    'label' => _opendata_formatter($lon, 'space'),
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
  );

  $map_fields[$title] = array(
    'id' => $title,
    'table' => $data_table_name,
    'field' => $title,
    'relationship' => 'none',
    'separator' => '',
    'exclude' => 1,
    'label' => _opendata_formatter($title, 'space'),
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 1,
      'path' => $detail_url, 
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
    'override' => array(
      'button' => 'Use default',
    ),
  );

  $map_fields[$subtitle] = array(
    'id' => $subtitle,
    'table' => $data_table_name,
    'field' => $subtitle,
    'label' => _opendata_formatter($subtitle, 'space'),
    'exclude' => 0,
    'relationship' => 'none',
    'separator' => '',
  );
        
  return $map_fields;
}

function _opendata_build_view_filter_fields($data_table_name, $colnames, $s_fields, $url, $table_key) {
	// Holds the built out views fields
	$s_selected_fields = array();

	// find out the table key column name
	$table_key_column = $colnames[$table_key];

	// a temporary display order field to hold, if the
	// user does not enter any display order
	$temp_display_order = 999;

	foreach ($colnames as $colname) {
		// Add the field_ prefix to the column name
		$selected_colname = 'field_selected_' . $colname;
		$sort_colname = 'field_display_order_' . $colname;
		
		// If the field is selected, build the view field
		if ($s_fields[$selected_colname] == 1) {
			$s_selected_fields[$colname] = array(
				'operator' => 'contains',
				'value' => '',
				'group' => '',
				'exposed' => TRUE,
				'expose' => array(
					'use_operator' => 0,
					'operator' => $colname . '_op',
					'identifier' => $colname,
					//'label' => $colname,
					'label' => _opendata_formatter($colname, "space"),
					'optional' => 1,
					'remember' => 0,   
				),
				//'case' => 1,
				'case' => 0,
				'id' => $colname,
				'table' => $data_table_name,
				'field' => $colname,
				'label' => _opendata_formatter($colname, 'space'),
			);

			if ($s_fields[$sort_colname] != '' or $s_fields[$sort_colname] > 0) {
				$s_selected_fields[$colname]['display_order'] = $s_fields[$sort_colname];
			} else {
				$s_selected_fields[$colname]['display_order'] = $temp_display_order;
				$temp_display_order++;
			}
		}
	}

	// sort the display order
	$s_selected_fields = _opendata_sort_display_order($s_selected_fields, 1);

	return $s_selected_fields;
}

function _opendata_build_view_summary_fields($data_table_name, $colnames, $s_fields, $url, $table_key, $detail_title) {
    
    // Holds the built out views fields
    $s_selected_fields = array();
    
    // IMPORTANT!
    // set the table key as the first field
    // to pass the right value to the detail page
    $s_selected_fields[$table_key . '_arg'] = array(
        'label' => $table_key . '_arg',
        'id' => $table_key . '_arg',
        'table' => $data_table_name,
        'field' => $table_key,
        'relationship' => 'none',
        'separator' => '',
        'exclude' => 1,
    );
    $s_selected_fields[$table_key . '_arg']['display_order'] = '-2';

		// set the detail page link title as the second field
    $s_selected_fields[$detail_title . '_arg'] = array(
        'label' => $detail_title . '_arg',
        'id' => $detail_title,
        'table' => $data_table_name,
        'field' => $detail_title,
        'relationship' => 'none',
        'separator' => '',
        'exclude' => 1,
    );
    $s_selected_fields[$detail_title . '_arg']['display_order'] = '-1';
    
    // a temporary display order field to hold, if the
    // user does not enter any display order
    $temp_display_order = 999;

    foreach ($colnames as $colname) {    
        // Add the field_ prefix to the column name
        $selected_colname = 'field_selected_' . $colname;
        $sort_colname = 'field_display_order_' . $colname;   
     
        // If the field is selected, build the view field
        if ($s_fields[$selected_colname] == 1) {
                        
            $s_selected_fields[$colname] = array(
                'id' => $colname,
                'table' => $data_table_name,
                'field' => $colname,
                'label' => _opendata_formatter($colname, 'space'),
                'exclude' => 0,
                'relationship' => 'none',
                'alter' => array(
                    'make_link' => 1,
                    'path' => $url,
                ),
            );
            
            //
            if ($s_fields[$sort_colname] != '' or $s_fields[$sort_colname] > 0) {
                $s_selected_fields[$colname]['display_order'] = $s_fields[$sort_colname];
            } else {
                $s_selected_fields[$colname]['display_order'] = $temp_display_order;
                $temp_display_order++;
            }
        }
    }
    
    // sort the display order
    $s_selected_fields = _opendata_sort_display_order($s_selected_fields, 1);

    //dpm($s_selected_fields);  
    
    return $s_selected_fields;
}

function _opendata_build_detail_view_fields($data_table_name, $colnames, $d_fields, $table_key, $do_map) {
	#dpm($d_fields);
	
	// Holds the built out views fields
	$d_selected_fields = array();
	
	// IMPORTANT!
	// set the table key as the first field
	// to pass the right value to the detail page
	$d_selected_fields[$table_key] = array(
	    'id' => $table_key,
	    'table' => $data_table_name,
	    'field' => $table_key,
	    'label' => _opendata_formatter($table_key, 'space'),
	    'exclude' => 0,
	    'relationship' => 'none',
	    'separator' => '',
	    'exclude' => 1,
	);
	$d_selected_fields[$table_key]['display_order'] = 0;
	
	// find out the table key column name
	$table_key_column = $colnames[$table_key];
	
	// a temporary display order field to hold, if the
	// user does not enter any display order
	$temp_display_order = 999;
	
	foreach ($colnames as $colname) {    
    // Add the field_ prefix to the column name
    $selected_colname = 'field_selected_' . $colname;
    $sort_colname = 'field_display_order_' . $colname;   
    $break_colname = 'field_line_break_' . $colname;   

    if ($d_fields[$selected_colname] == 1) {
			if ($d_fields[$break_colname] == 1) {
				$strbreak = '<div class="clear"></div>';
				$strstyle = '<div class="detail wide">'; 
			} else {
				$strbreak = '';
				if ($do_map == 1) {
					$strstyle = '<div class="detail">'; 
				} else {
					$strstyle = '<div class="detail-wide">'; 
				}
			}

			if ($d_fields[$break_colname] == 1) {
			}

			// construct custom text field
			$display_colname = _opendata_formatter($colname, 'space');
			$strtext = $strstyle; 
			$strtext .= '<strong>' . $display_colname . '</strong>';
			$strtext .= '<p>[' . $colname . ']</p>';
			$strtext .= '</div>';
			$strtext .= $strbreak;

	    $d_selected_fields[$colname] = array(
	        'id' => $colname,
	        'table' => $data_table_name,
	        'field' => $colname,
	        'label' => '',
	        'exclude' => 0,
	        'relationship' => 'none',
	        'alter' => array(
	            'alter_text' => 1,
							'text' => $strtext,
	        ),
	    );
	    
	    if ($d_fields[$sort_colname] != '' or $d_fields[$sort_colname] > 0) {
				$d_selected_fields[$colname]['display_order'] = $d_fields[$sort_colname];
	    } else {
				$d_selected_fields[$colname]['display_order'] = $temp_display_order;
				$temp_display_order++;
	    }
    }
	}
	
	// sort the display order
	$d_selected_fields = _opendata_sort_display_order($d_selected_fields);
	
	#dpm($d_selected_fields);  
	
	return $d_selected_fields;
}

function _opendata_sort_display_order($selected_fields, $isSummary = 0) {

	foreach ($selected_fields as $key => $row) {
		$selected_field[$key] = $row['display_order'];
	}

	array_multisort($selected_field, SORT_ASC, $selected_fields);

	// remove path for only summary fields
	if ($isSummary) {
		// hold index of the array
		$i = 0;
	
		foreach ($selected_fields as $key => $value) {        
			if ($i > 2) {
				// delete the 'path' array item				
				//unset($selected_field['alter']['path']);
				unset($selected_fields[$key]['alter']['path']);
			}
			// increment the current index
			$i++;
		}
	}
	#dpm($selected_fields);

	return $selected_fields;
}

function _opendata_build_style_options($colnames, $fields) {

    $style_options = array();
    $style_options_columns = array();
    $style_options_info = array();
    
    foreach ($colnames as $colname) {    
        //Add the field_ prefix to the column name
        $selected_colname = 'field_selected_' . $colname;
        $sortable_colname = 'field_sortable_' . $colname;
        
        //If the field is selected, build the view field
        if ($fields[$selected_colname] == 1) {
            
            //Add it to the columns
            $style_options_columns[$colname] = $colname;
                        
            //Add it to the info
            if ($fields[$sortable_colname] == 1) {
              $style_options_info[$colname] = array ('sortable' => 1, 'separator' => '',);
            } else {
              $style_options_info[$colname] = array();
            }
        }
    }
    
    $style_options = array (
      'grouping' => '',
      'override' => 1,
      'sticky' => 1,
      'order' => 'asc',
      'columns' => $style_options_columns,
      'info' => $style_options_info,
      'default' => -1,
    );
    
    #Debug
    #dpm($style_options);
    
    return $style_options;
}

/**
 * A helper function to build the fields of the columns
 * 
 * @param
 *   Selected table ID
 *
 * @return 
 *   A form array
 */
function _opendata_build_summary_listing($tableid) {
  //Debug
  //dpm($tableid);
  
  $form['summary_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields to Include on Summary Listings'),
    '#tree' => TRUE,
    '#prefix' => '<div class="opendata-summary-fields">',
    '#suffix' => '</div>',
  );
  
  // check all checkbox
  $form['summary_fields']['checkall'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check all fields'),
    '#attributes' => array(
      'onclick' => 'checkall("summary_fields_checkbox")',
      'class' => 'checkall_checkbox',
    ),
    '#prefix' => '<div class="clear-fields">',
    '#suffix' => '</div>',
  );
  $form['summary_fields']['lbl_display_fields'] = array(
      '#type' => 'item',
      '#title' => t('Summary Listing Fields'),
  );
  $form['summary_fields']['lbl_display_order'] = array(
      '#type' => 'item',
      '#title' => t('Display Field'),
  );
  $form['summary_fields']['lbl_allow_search'] = array(
      '#type' => 'item',
      '#title' => t('Allow Search?'),
  );
  $form['summary_fields']['lbl_sortable'] = array(
      '#type' => 'item',
      '#title' => t('Sortable?'),
  );
  $form['summary_fields']['lbl_exportable'] = array(
      '#type' => 'item',
      '#title' => t('Exportable?'),
  );
  
  //Build the columns
  $colnames = array();
  $colnames = _opendata_get_fields($tableid);
  
  // Build the fields
  $fields = array();
  
  foreach ($colnames as $colname) {
    $fields[$colname] = array(
      'id' => $colname,
      'table' => $rawtablename,
      'field' => $colname,
      'label' => $colname,
      'exclude' => 0,
      'relationship' => 'none',
    );
    
    //Generate the table fields checkboxes
    $selected = 'field_selected_' . $colname;
    $form['summary_fields'][$selected] = array(
      '#type' => 'checkbox', 
      '#title' => t($colname),
      '#attributes' => array(
        'class' => 'summary_fields_checkbox',
      ),
    );
    
    //Generate Sort order fields
    $display_order = 'field_display_order_' . $colname;
    $form['summary_fields'][$display_order] = array(
      '#type' => 'textfield', 
      '#size' => 10
    );
    
    //Generate filter options
    $filter_options = 'field_filter_' . $colname;
    $form['summary_fields'][$filter_options] = array(
      '#type' => 'select',
      '#options' => array(
          'na'=>'',
          'textfield'=>'Text Search',
          'select'=>'Dropdown',
          'checkbox'=>'Checkboxes',
      ),
    );
    
    //Generate the sortable checkboxes
    $sortable = 'field_sortable_' . $colname;
    $form['summary_fields'][$sortable] = array('#type' => 'checkbox', '#label' => 'Sortable');

    // generate exportable checkboxes 
    $exportable = 'field_exportable_' . $colname;
    $form['summary_fields'][$exportable] = array(
			'#type' => 'checkbox', 
			'#label' => 'exportable',
			//'#default_value' => variable_get('exportable', 1),
		);
  }
  
  //Debug
  //dpm($form);
  
  return $form;
}

/**
 * A helper function to build the fields of the columns
 * 
 * @param
 *   Selected table ID
 *
 * @return 
 *   A form array
 */
function _opendata_build_detail_listing($tableid) {
    
	$form['detail_fields'] = array(
		'#type' => 'fieldset',
		'#title' => t('Fields to Include on Detail Page'),
		'#tree' => TRUE,
		'#prefix' => '<div class="opendata-detail-fields">',
		'#suffix' => '</div>',
	);

	// check all checkbox
	$form['detail_fields']['checkall'] = array(
		'#type' => 'checkbox',
		'#title' => t('Check all fields'),
		'#attributes' => array(
			'onclick' => 'checkall("detail_fields_checkbox")',
			'class' => 'checkall_checkbox',
		),
		'#prefix' => '<div class="clear-fields">',
		'#suffix' => '</div>',
	);

	//Build the columns
	$colnames = array();
	$colnames = _opendata_get_fields($tableid);
    
  $form['detail_fields']['lbl_display_fields'] = array(
    '#type' => 'item',
    '#title' => t('Detail Listing Fields'),
  );
  $form['detail_fields']['lbl_display_order'] = array(
    '#type' => 'item',
    '#title' => t('Display Field'),
  );
	/*
  $form['detail_fields']['lbl_title'] = array(
    '#type' => 'item',
    '#title' => t('Title'),
  );
	*/
  $form['detail_fields']['lbl_line_break'] = array(
    '#type' => 'item',
    '#title' => t('Line Break'),
  );
  $form['detail_fields']['lbl_badge'] = array(
    '#type' => 'item',
    '#title' => t('Badge'),
  );
    
  // Build the fields
  $fields = array();
    
  foreach ($colnames as $colname) {
    $fields[$colname] = array(
      'id' => $colname,
      'table' => $rawtablename,
        'field' => $colname,
        'label' => $colname,
        'exclude' => 0,
        'relationship' => 'none',
      );
      
      //Generate the table fields checkboxes
      $selected = 'field_selected_' . $colname;
      $form['detail_fields'][$selected] = array(
        '#type' => 'checkbox', 
        '#title' => t($colname),
        '#attributes' => array(
          'class' => 'detail_fields_checkbox',
        ), 
      );
      
      //Generate Sort order fields
      $display_order = 'field_display_order_' . $colname;
      $form['detail_fields'][$display_order] = array('#type' => 'textfield', '#size' => 10);

      // generate the line break
      $line_break = 'field_line_break_' . $colname;
      $form['detail_fields'][$line_break] = array('#type' => 'checkbox');
      
		// generate the badge field
		$badge = 'field_badge_' . $colname;
		$form['detail_fields'][$badge] = array('#type' => 'checkbox');
	}
    
	//dpm($form);
	return $form;
}

/**
 * A helper function for getting the columns
 * 
 * @param
 *   Table name
 *
 * @return 
 *   Column names 
 */
function _opendata_get_fields($table_name) {
	$twtid = _opendata_get_twtid($table_name);
	//$twtid = '26';
	$pk = NULL;
	$colnames = array();
	$sql = "SELECT colname, primarykey
	        FROM {tw_columns}
	        WHERE ignorecol=0 AND twtid = '%d' 
	        ORDER BY weight";
	$colresult = db_query($sql, $twtid);
	          
	while ($colrow = db_fetch_object($colresult)) {
	  $colnames[] = $colrow->colname;
	  if ($colrow->primarykey) {
	    $pk = $colrow->colname;
	  }
	}
	
	#debug
	//dpm($colnames);
	
	return $colnames;
}

function _opendata_get_twtid($table_name) {
	$sql = "SELECT twtid 
	        FROM {tw_tables}
	        WHERE tablename = '%s'";
	$twtid = db_result(db_query($sql, $table_name));
	          
	return $twtid;
}

/**
 * A helper function for getting the primary key of a table
 * 
 * @param
 *   Table name
 *
 * @return 
 *   primary key 
 */
function _opendata_get_primary_key($tablename) {
	$twtid = _opendata_get_twtid($tablename);
	$pk = NULL;
	$colnames = array();
	$sql = "SELECT colname, primarykey
	        FROM {tw_columns}
	        WHERE ignorecol=0 AND twtid = '%d' 
	        ORDER BY weight";
	$colresult = db_query($sql, $twtid);
	          
	while ($colrow = db_fetch_object($colresult)) {
	  //$colnames[] = $colrow->colname;
	  if ($colrow->primarykey) {
	    //$pk = $colrow->colname;
			// we only want to add the primary key field
			//$colnames[] = $colrow->colname;
			$colnames[$colrow->colname] = $colrow->colname;
	  }
	}
	//dpm($colnames);
	return $colnames;
}

// TODO: get tablename from tw_tables by twtid 
function _opendata_get_tablename_by_twtid($twtid) {
}

// we only want to adopt tables in tw if it is not 
// already adopted. 
// this function is used by opendata_summary
function _opendata_exists_in_tw($tablename) {
	$sql = "SELECT twtid 
	        FROM {tw_tables}
	        WHERE tablename = '%s'";
	$result = db_result(db_query($sql, $tablename));

	//dpm($result);
	if ($result) {
		return true;
	} else {
		return false;
	}
}

// for each table name, we want to check if there is a primary key. 
// this function is used by _opendata_get_tables()
function _opendata_has_pk($tablename) {
  $inspect = schema_invoke('inspect');
  $schema = $inspect[$tablename];

  if (!$schema) {
    drupal_set_message(t('Could not analyze !tablename',
      array('!tablename' => $tablename)), 'error');
		return false;
  } else {
    if (isset($schema['primary key'])) {
			return true;
    } else {
			return false;
		}
	}
}

/**
 * A helper function for getting the table names
 * 
 * @param
 *
 * @return 
 *   Array of table names
 */
function _opendata_get_tables() {
	$tablenames = array();
	$sql = "SHOW TABLES LIKE 'ed_%'";
	$result = db_query($sql);
	
	$options = array();
	while ($row = db_fetch_array($result)) {
		foreach ($row as $tablename) {
			$unprefixed = schema_unprefix_table($tablename);

			if (_opendata_has_pk($unprefixed)) {
				$options[$unprefixed] = $unprefixed;
			}
		}
	}
	return $options;
}

/**
 * A helper function for getting the table names
 * 
 * @param
 *   Selected table ID
 *
 * @return 
 *   Selected table name
 */
function _opendata_get_tablename($tableid) {
	/*
	$data_table_names = array();
	$data_table_names = _opendata_get_tables();
	$data_table_name = $data_table_names[$tableid];
	
	return $data_table_name;
	*/
	$tablename = $tableid;
	return $tablename;
}

function _opendata_build_desc_fields($more_info_url, $desc_brief, $desc_extended) {
  // instantiate an array to hold the final fields structure 
  $desc_fields = array();

	if ($more_info_url != '') {
		$desc_fields['markup'] = array(
			'label' => '',
			'alter' => array(
				'alter_text' => 0,
				'text' => '',
				'make_link' => 0,
				'path' => '',
				'link_class' => '',
				'alt' => '',
				'prefix' => '',
				'suffix' => '',
				'target' => '',
				'help' => '',
				'trim' => 0,
				'nl2br' => 0,
				'max_length' => '',
				'word_boundary' => 1,
				'ellipsis' => 1,
				'html' => 0,
				'strip_tags' => 0,
			),
			'empty' => '',
			'hide_empty' => 0,
			'empty_zero' => 0,
			'value' => '<div id="opendata-more-info"><a href="' . $more_info_url . '"><div id="opendata-more-info-link"></div></a></div>',
			'format' => '2',
			'exclude' => 0,
			'id' => 'markup',
			'table' => 'customfield',
			'field' => 'markup',
			'override' => array(
				'button' => 'Use default',
			),
			'relationship' => 'none',
		);
	}

	$desc_fields['markup_1'] = array(
		'label' => '',
		'alter' => array(
			'alter_text' => 0,
			'text' => '',
			'make_link' => 0,
			'path' => '',
			'link_class' => '',
			'alt' => '',
			'prefix' => '',
			'suffix' => '',
			'target' => '',
			'help' => '',
			'trim' => 0,
			'nl2br' => 1,
			'max_length' => '',
			'word_boundary' => 1,
			'ellipsis' => 1,
			'html' => 0,
			'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'value' => '<div id="desc-brief" style="padding-top: 1em;">' . $desc_brief . '</div>',
		'format' => '2',
		'exclude' => 0,
		'id' => 'markup_1',
		'table' => 'customfield',
		'field' => 'markup',
		'override' => array(
			'button' => 'Use default',
		),
		'relationship' => 'none',
	);

	if ($desc_extended != '' && $desc_extended != null) {
		$desc_fields['markup_2'] = array(
			'label' => '',
			'alter' => array(
				'alter_text' => 0,
				'text' => '',
				'make_link' => 0,
				'path' => '',
				'link_class' => '',
				'alt' => '',
				'prefix' => '',
				'suffix' => '',
				'target' => '',
				'help' => '',
				'trim' => 0,
				'nl2br' => 1,
				'max_length' => '',
				'word_boundary' => 1,
				'ellipsis' => 1,
				'html' => 0,
				'strip_tags' => 0,
			),
			'empty' => '',
			'hide_empty' => 0,
			'empty_zero' => 0,
			'value' => '<div id="desc-extended" style="margin-top: 10px;">' . $desc_extended . '</div><button id="desc-button">More</button>',
			'format' => '2',
			'exclude' => 0,
			'id' => 'markup_2',
			'table' => 'customfield',
			'field' => 'markup',
			'override' => array(
				'button' => 'Use default',
			),
			'relationship' => 'none',
		);
	}

	$desc_fields['phpcode'] = array(
		'label' => '',
		'alter' => array(
			'alter_text' => 0,
			'text' => '',
			'make_link' => 0,
			'path' => '',
			'link_class' => '',
			'alt' => '',
			'prefix' => '',
			'suffix' => '',
			'target' => '',
			'help' => '',
			'trim' => 0,
			'nl2br' => 0,
			'max_length' => '',
			'word_boundary' => 1,
			'ellipsis' => 1,
			'html' => 0,
			'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
    'value' => '<script type="text/javascript">
$(document).ready(function() {
  $(\'#desc-extended\').hide();
  $(\'#desc-button\').click(function() {
    $(\'#desc-extended\').toggle(300, function() {
      if ($(\'#desc-button\').text() == \'Less\') {
        $(\'#desc-button\').text(\'More\');
      } else {
        $(\'#desc-button\').text(\'Less\');
      }
    });
  });
});
</script>',
		'exclude' => 0,
		'id' => 'phpcode',
		'table' => 'customfield',
		'field' => 'phpcode',
		'override' => array(
			'button' => 'Use default',
		),
		'relationship' => 'none',
	);

	$desc_fields['markup_3'] = array(
		'label' => '',
		'alter' => array(
			'alter_text' => 0,
			'text' => '',
			'make_link' => 0,
			'path' => '',
			'link_class' => '',
			'alt' => '',
			'prefix' => '',
			'suffix' => '',
			'target' => '',
			'help' => '',
			'trim' => 0,
			'nl2br' => 1,
			'max_length' => '',
			'word_boundary' => 1,
			'ellipsis' => 1,
			'html' => 0,
			'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'value' => '<div style="margin-top: 0.5em"><a href="#table">Skip to table</a></div>',
		'format' => '2',
		'exclude' => 0,
		'id' => 'markup_3',
		'table' => 'customfield',
		'field' => 'markup',
		'override' => array(
			'button' => 'Use default',
		),
		'relationship' => 'none',
	);

  return $desc_fields;
}

/**
 * A helper function for formatting strings to desired type
 * 
 * @param
 *   The preset title provided by the user
 *
 * @return 
 *   A formatted text string
 */
function _opendata_formatter($val, $type) {
  switch ($type) {
    case 'lower':
      $val = strtolower($val);
      break;
    case 'hyphen':
      $val = str_replace(" ", "-", strtolower($val));
      break;
    case 'html':
      nl2br($val);
      break;
    case 'underscore':
      $val = str_replace(" ", "_", strtolower($val));
      break;
    case 'space':
      $val = str_replace("_", " ", ucwords($val));
      // split CamelCase
      $val = preg_split('/([[:upper:]][[:lower:]]+)/', $val, null, PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY);
      $val = implode(" ", $val);
      break;
  }

  return $val;
}

