<?php

/**
 * @file
 * This file contains helper functions for upcoming_movies_list module. 
 */

/**
 * Returns a timestamp of the release date of the current upcoming movies.
 * @return type
 * Timestamp of current release date.
 */
function fetch_upcoming_movies_release_date() {

    // Fetch release date from imdb
    $url_new_movies_list = "http://www.imdb.com/movies-in-theaters/?pf_rd_m=A2FGELUUNOQJNL&pf_rd_p=2750721702&pf_rd_r=11DRXYJX1PECE8HDED97&pf_rd_s=right-2&pf_rd_t=15061&pf_rd_i=homepage&ref_=hm_otw_sm";
    $homepage = file_get_contents($url_new_movies_list);
    $string_pattern_opening_date = ("'Opening This Week - (.*?)&nbsp;</h3>'");
    preg_match($string_pattern_opening_date, $homepage, $match);
    $upcoming_movies_release_date = $match[1];
    
    //Create markup to format to timestamp.
    $date = new DateTime();
    $timestamp = $date->getTimestamp();
    $year_of_release_date = "2015";
    $release_date_markup = $upcoming_movies_release_date . " " . $year_of_release_date;
    $release_date_timestamp = strtotime($release_date_markup);
    return $release_date_timestamp;
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
 * Updates last_checked_upcoming_movies varibale to current time.
 */
function update_last_time_updated_var() {   
    $date = new DateTime();
    $timestamp = $date->getTimestamp();
    $save_date = format_date($timestamp, $type = 'medium', $format = 'd.m.Y H:i:s', 'Europe/Berlin');
    $new_time_of_update_string = $save_date . " Berlin time";
    variable_set('last_checked_upcoming_movies', $new_time_of_update_string);
}

/**
 * Saves the current upcoming movies as nodes from imdb.
 */
function save_upcoming_movie_nodes() {
    // Create markup to save release date of fetched movie titles.
    $release_date_timestamp = fetch_upcoming_movies_release_date();
    $upcoming_movies = fetch_upcoming_movie_titles();
    global $user;
    // Save newly fetched movie titles in nodes.
    foreach ($upcoming_movies as $movie_title) {
        $node = new stdClass();
        $node->title = $movie_title;
        $node->field_release_date['und'][0]['value'] = $release_date_timestamp;
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
 * Checks if date is already saved in one of the upcoming_movie nodes release date.
 */
function is_release_date_saved($release_date_timestamp) {
    $select = db_select('field_revision_field_release_date', 't')
        ->fields('t')
        ->groupBy('t.field_release_date_value');
    if ($result_query = $select->execute()) {
        $release_dates = array();
        while ($array = $result_query->fetchAssoc()) {
            $release_dates[$array['field_release_date_value']] = $array['field_release_date_value'];
        }
        if (in_array($release_date_timestamp, $release_dates)) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}
