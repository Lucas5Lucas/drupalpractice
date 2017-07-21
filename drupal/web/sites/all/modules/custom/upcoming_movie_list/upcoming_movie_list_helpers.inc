<?php

/**
 * @file
 * This file contains helper functions for upcoming_movies_list module 
 */


/**
 * Returns a string with the release date of the current upcoming movies.
 *
 * @params string 
 *   release date of the current upcoming movies.
 */
function fetch_upcoming_movies_release_date() {

    // Fetch release Date from imdb
    $url_new_movies_list = "http://www.imdb.com/movies-in-theaters/?pf_rd_m=A2FGELUUNOQJNL&pf_rd_p=2750721702&pf_rd_r=11DRXYJX1PECE8HDED97&pf_rd_s=right-2&pf_rd_t=15061&pf_rd_i=homepage&ref_=hm_otw_sm";
    $homepage = file_get_contents($url_new_movies_list);
    $string_pattern_opening_date = ("'Opening This Week - (.*?)&nbsp;</h3>'");
    preg_match($string_pattern_opening_date, $homepage, $match);
    $upcoming_movies_release_date = $match[1];
    
    //Create markup to save release date of fetched movie titles.
    $date = new DateTime();
    $timestamp = $date->getTimestamp();
    $year_of_release_date = format_date($timestamp, $type = 'medium', $format = 'Y', 'Europe/Berlin');
    $release_date_markup = $upcoming_movies_release_date . " " . $year_of_release_date;
    return $release_date_markup;
}

/**
 * Returns an array with all the current upcoming movie titles from imdb.
 *
 * @params string 
 *   An array with all the upcoming movie titles.
 */
function fetch_upcoming_movie_titles() {
    $url_new_movies_list = "http://www.imdb.com/movies-in-theaters/?pf_rd_m=A2FGELUUNOQJNL&pf_rd_p=2750721702&pf_rd_r=11DRXYJX1PECE8HDED97&pf_rd_s=right-2&pf_rd_t=15061&pf_rd_i=homepage&ref_=hm_otw_sm";
    $homepage = file_get_contents($url_new_movies_list);
    $string_pattern = "'list detail sub-list(.*?)list detail sub-list'si";
    
    preg_match($string_pattern, $homepage, $match);
    preg_match_all('/title=\"(.*?)\" itemprop/', $match[1], $extracted_titles);
    $upcoming_movie_titles = array();
    $counter = 0;
    foreach ($extracted_titles[1] as $title) {
        $upcoming_movie_titles[$counter] = $title;
        $counter++;
    }
    return $upcoming_movie_titles;
}

/**
 * Updates last_time_updated_node to current time
 */
function update_last_time_updated_node() {
    
    $date = new DateTime();
    $timestamp = $date->getTimestamp();
    $save_date = format_date($timestamp, $type = 'medium', $format = '', 'Europe/Berlin');
    $new_time_of_update = $save_date . " Berlin time";
    variable_set('last_checked_upcoming_movies', $new_time_of_update);
}

/**
 * Saves the current upcoming movies as nodes from imdb
 */
function save_upcoming_movie_nodes() {
    // Create markup to save release date of fetched movie titles
    $release_date_markup = fetch_upcoming_movies_release_date();
    $upcoming_movies = fetch_upcoming_movie_titles();
    global $user;
    // Save newly fetched movie titles in nodes.
    foreach ($upcoming_movies as $movie_title) {
        $node = new stdClass();
        $node->title = $movie_title;
        $node->field_release_date['und'][0]['value'] = $release_date_markup;
        $node->type = "upcoming_movie";
        node_object_prepare($node);
        $node->language = LANGUAGE_NONE;
        $node->uid = $user->uid;
        $node->status = 1;
        $node->promote = 1;
        $node->comment = 0;
        $node = node_submit($node);
        node_save($node);
    }
}

/**
 * Checks if string is already saved as release date in database
 */
function is_release_date_saved($release_date_markup) {
    // Get all upcoming_movie_release_date nodes.
    $result_release_date_query = db_query("SELECT nid FROM {node} AS n WHERE n.type = 'upcoming_movie_release_date'");

    if (!empty($result_release_date_query)) {
        $old_release_dates = array();
        foreach ($result_release_date_query as $already_saved_release_date) {
            $upcoming_movie_release_date_nids[] = $already_saved_release_date->nid;
        }
        $nodes = node_load_multiple($upcoming_movie_release_date_nids, 'teaser');
        // Fetch the title-values.
        $old_dates = array();
        foreach ($nodes as $node) {
            $old_dates[] = $node->title;
        }
        if (in_array($release_date_markup, $old_dates)) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

/**
 * Stores the passed string as a release_date node
 */
function save_release_date($release_date_markup) {
    global $user;
    // add new node with new release date
    $node = new stdClass();
    $node->title = $release_date_markup;
    $node->type = "upcoming_movie_release_date";
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node = node_submit($node);
    node_save($node);
}