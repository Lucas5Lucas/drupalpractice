<h2><?php echo "Release Date: ".$release_date; ?></h2>
<ol>
    <?php
    foreach ($movietitles as $title) {
        print "<li><h2>" . $title . "</h2></li>";
    }
    ?>   
</ol>
<h3><?php print "Last checked: ".$time_of_update_markup;?></h3>