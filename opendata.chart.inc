<?php 

/**
 * Creats a pie or bar chart using the amCharts module 
 * 
 * @param &$view 
 *		A reference to the $view object being built. 
 *
 * @param $dataset_name 
 *		The name or accronym of the dataset (i.e. PN, RTL, TAH, etc)
 * 
 * @param $chart_type 
 *		The chart type (i.e. pie or bar) 
 * 
 * @param $chart_title 
 *		The title to be displayed at the top of the chart (i.e. Share of Total) 
 * 
 * @param $series_fields 
 *		The series to be tallied for the chart. If for instance, you specify "nid" 
 *		then the charts would be grouped on node id's 
 * 
 * @param $x_labels 
 *		The labels to show 
 *
 * @return 
 *		This function does not return anything. It modifies the $view object 
 *		passed by reference. 
 * 
 */
function opendata_create_chart(&$view, $dataset_name, $chart_type, $chart_title, $series_fields, $x_labels) {
	if ($chart_type == "pie" || $chart_type == "bar") {	
		if ($chart_type == "pie") {	
			$handler = $view->new_display('block', 'Pie Chart', 'block_1');
		} else {
			$handler = $view->new_display('block', 'Bar Graph', 'block_2');
		}
		//$handler->override_option('title', 'Share of Total*');
		//$handler->override_option('title', 'Total (Counts)*');
		$handler->override_option('title', "$chart_title");
		$handler->override_option('header_format', '2');
		$handler->override_option('header_empty', 0);
		$handler->override_option('style_plugin', 'charts');
		$handler->override_option('style_options', array(
		  'grouping' => '',
		  'views_charts_series_fields' => array(
		    //'nid' => 'nid',
				"$series_fields" => "$series_fields",
		  ),
		  //'views_charts_x_labels' => 'name',
		  'views_charts_x_labels' => "$x_labels",
		  'width' => '700',
		  'height' => '400',
		  'engine' => 'amcharts',
		  'type' => array(
		    //'amcharts' => 'pie',
		    'amcharts' => "$chart_type",
		  ),
		  'wmode' => 'window',
		  'y_min' => '',
		  'y_max' => '',
		  'y_step' => '',
		  'y_legend' => '',
		  'background_colour' => '',
		  'series_colours' => '',
		));
		$handler->override_option('exposed_block', FALSE);
		//$handler->override_option('block_description', 'PN Pie Chart Block');
		//$handler->override_option('block_description', 'PN Bar Graph Block');
		$handler->override_option('block_description', "Block: $dataset_name $chart_type chart");
		$handler->override_option('block_caching', -1);
	}
}
