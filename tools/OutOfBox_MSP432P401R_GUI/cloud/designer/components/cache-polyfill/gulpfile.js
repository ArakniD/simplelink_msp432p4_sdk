var gulp = require('gulp');
var concat = require('gulp-concat');

gulp.task('default', function() {
  return gulp.src('./lib/caches.js')
    .pipe(concat('serviceworker-cache-polyfill.js'))
    .pipe(gulp.dest('./dist/'));
});