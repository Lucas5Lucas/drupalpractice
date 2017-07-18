<ol>
    <?php
    foreach ($movietitles as $title) {
        print "<li><h2>" . $title . "</h2></li>";
    }
    ?>   
</ol>
<h3><?php print "Last updated: ".$time_of_update_markup;?></h3>