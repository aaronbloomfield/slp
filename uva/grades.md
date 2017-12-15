SLP Grading Metric
==================

[Go up to the main UVa page](index.html) ([md](index.md))

This course is somewhat of a challenge to grade, as there is very little graded work in the traditional sense.  Nonetheless, here is how everything is graded for the fall 2017 semester.  The details herein follow the grading guidelines described in the [fall 2017 course introduction](course-introduction-fall.html#/grades), with two slight modifications, described herein (search for 'difference' in this document if you want to find them quickly).


## Promises

First, there were two promises that were made to the class:

- Getting all 'good' ratings on the iteration evaluations, and doing an 'average' amount of work, would lead to an 'A', assuming everything else was in line (meaning everything else was an 'A' grade).  This implies that everybody could (potentially) receive an A in the class.
- The number of hours you reported were never used in the grade calculation -- only whether you reported times in a given iteration or not.

Both of these were strictly adhered to.

## Work-done and the grading factor

Two of the scores were project-based: the iteration evaluations and the final group project rating.  The reason is that a student who does little work in an otherwise great group should not get as much credit for the quality of the system as somebody who does great work.  Likewise, a great student in a weaker group should get more credit than students who do less work.

To determine this, all students are assigned a "work-done" rating, which is based on how much work we feel the student has done during the semester.  This was originally based on a Likert scale (integer values from 1 to 5), but we realized that values between these were necessary as well.  Thus, the scale is as follows.

| rating as text | rating as a number | factor |
|----------------|--------------------|--------|
| well below average | 1 | 0.35 |
| a bit worse than below average  | 1.5 | 0.55 |
| below average | 2 | 0.75 |
| slightly below average | 2.5 | 0.875 |
| ever so slightly below average | 2.75 | 0.9375 |
| average | 3 | 1 |
| ever so slightly above average | 3.25 | 1.0375 |
| slightly above average | 3.5 | 1.075 |
| above average | 4 | 1.15 |
| a bit more than above average | 4.5 | 1.225 |
| well above average | 5 | 1.3 |

Yes, the names need a bit of work.  And keep in mind that an 'average' rating is on track to get an 'A'.

The last column is the *factor*, which is used to scale the group scores.  Thus, if a group receives a 80% on something, a student with an 'average' rating would receive the full 80%.  A student with a 'below average' rating would receive 0.75\*80% = 60% for that part.  Likewise, a student with a 'well above average' would receive 1.3\*80 = 104% for that part.

Note that a 'well below average' grade is an automatic failure for the course, regardless of any other grades.  One can still fail with a score other than 'well below average'.  It has always been the case, however, that students with a 'well below average' were clearly in the failure category anyway.  Historically, it has been very rare for that to be issued a 'well below average' rating in the spring, as those students typically do not pass the fall semester.

## Iteration evaluations

The progress done on the iteration evaluations were graded out of 4 points: 0 was no progress, 1 was poor, 2 was fair, 3 was good, 4 was excellent.  Note that a 0 (none) has never (yet) been issued.  Also note that the last iteration in a semester typically does not have an iteration evaluation rating, as that is encapsulated into the overall project evaluation.

The iteration evaluations were averaged to yield the evaluation score, and that average is shown in the grading metrics.  An average of 1.0 (all poor ratings) would be 0% for this part, and an average of 3.0 (all average ratings) would be 93% (an A); this scale is linear.  Thus, the conversion formula is:

percent = (average - 1) x 93.0/2.0 = (average - 1) x 46.5


Thus, all 'good' evaluations would yield (3.0-1) x 46.5 = 93%, which is an 'A'.  An average evaluation score of 2.5 (half 'good' and half 'fair' evaluations) would result in (2.5-1) x 46.5 = 69.75%.  As this is a group score, it is multiplied by the factor from above.

## Leadership score

The leadership score is graded out of 10 points, and was based on the results of one's leadership iterations; these are shown in the evaluations tab of the course grading tool.  One could be the secretary, lead, both, or neither in a given semester.  Note that the 'neither' situation could only occur in the fall, and only for groups where there were more than six members.

- Secretary only:
    - Each iteration is graded out of 5 points
    - Each iteration with no reports entered is 0/5
    - For the first and last iterations: because there are expected to be fewer meetings: 1+ report filed is 5/5 points, 0 reports is 2/5 points
    - For the middle iterations: 1 report filed is 4/5 points, 2+ reports are 5/5 points
    - If they did three iterations, then the score is out of 15, and is scaled down to be out of 10, rounded to the nearest 1/2
- Lead only:
    - Each late report is 2 points off
    - Each missing report is 5 points off; thus, missing 2 means a zero for the leadership score, regardless of anything else
    - If they did three iterations, then scale down, by 2/3rds, the amount off from 10, then round to the nearest 1/2
- Both roles:
    - Grade each separately and take the average, rounding to the nearest 1/2
    - If they did a reasonably good job on both, or they had a lot of leadership iterations, they get +1 point due to the amount of leadership they did
    - A leadership score of 9.5 rounds up to 10; else, it stays at the x.5
- Neither roles:
    - In this case, the overall course grade is scaled so that the leadership score does not count; see below for details.

Note that taking really poor notes would count as no notes taken for that occurrence.

## Grade components

The spring grades were computed as follows:

- 45%: iteration evaluations (described above), multiplied by the factor
- 25%: final project evaluation (typically out of 10 points), multiplied by the factor
- 10%: project in lieu of the final exam (out of 10 points)
- 10%: homework grades
- 5%: leadership score (described above), out of 10 points
- 5%: journaling entries (averaging the scores, each of which was out of 5)

One of the differences from the [fall 2017 course introduction](course-introduction-fall.html#/grades) was that instead of counting leadership for 10%, we counted it for 5% and the journal entries for 5%.  Note that only the first two items were group work.  The last four items were were individual work, and thus are not multiplied by the factor.

The result of the above is called the *base score*.

## Penalties

There were a few things that would deduct from one's grade.  With one exception, these were all clearly stated throughout the semester, as well as the [course syllabus](syllabus.html) ([md](syllabus.md)) and the grades section of the [fall course introduction](course-introduction-fall.html#/grades).

- Unexcused absences (recall that attendance is required) was a 3 percentage point penalty for *each* occurrence.  This applies to absences with no excuse; excused absences do not incur a penalty.  Is this too harsh?  Not really -- when one is in a job outside the university, one cannot just simply miss work without explaining why, or not perform a required job function, without a consequence.
- Not reporting hours was a 3 percentage point penalty for *each* occurrence.  This is whether one reported hours or not -- how many hours reported was not factored into the grade in any way.
- Missing a survey incurred a 10 percentage point penalty.  However, this almost never occurred, as everybody is reminded repeatedly -- and we typically wait until all the surveys before closing them.
- Not submitting 5 test cases in the required iterations (3, 4, and 6) incurred a 1 percentage point penalty.  This was the other difference from what was in the [fall 2017 course introduction](course-introduction-fall.html#/grades).

The 'base score' (from above), minus the penalties, is the *final course average*.

## Final course grade

This course used the standard decade scale.  The final course average was rounded up (if appropriate), and the following table was used to compute the final course grade.

| minimum score | letter grade |
|---------------|--------------|
| -&#x221e; | F |
| 60 | D- |
| 63 | D |
| 67 | D+ |
| 70 | C- |
| 73 | C |
| 77 | C+ |
| 80 | B- |
| 83 | B |
| 87 | B+ |
| 90 | A- |
| 93 | A |
| ??? | A+ |

Some notes on this:

- Due to the scaling of the grade based on the work-done rating and the factor, one can have scores in excess of 100%.  In particular, students in the past have had scores exceeding 150% (although that was only one time).
- One can also have a negative score (typically due to a poor work-done rating and lots of penalties) -- this has also happened, but is also rare.
- Note that a 'well below average' grade is an automatic failure for the course, regardless of any other grades; one can still fail with a score other than 'well below average'.
- An 'A+' is only issued to students with that meet *two* criteria: a 'well above average' work-done rating *and* a high enough average.  Note that the A+ range is a percentage of the class (typically the top 3% to 5% of the class), so the cut-off varies from semester to semester.

## Analysis

One may claim that this metric is too harsh. Perhaps, but it does take into account, in a formulaic way, all the factors that contribute to the grade.  And students had chance to challenge every component that is used in the grade determination.

For the fall 2017 semester, these were the grade distributions:

- 44% are some sort of A (A-, A, or A+); 3 of those were an A+
- 21% are some sort of B
- 15% are some sort of C
- 6% are some sort of D
- 13% are an F

Those percentages are higher than a typical 4th year course, so the distribution of grades is reasonable in comparison to other 4th year courses.

Also, it turned out this semester that the average work-done rating was *exactly* 3.0.  We weren't aiming for that, it just turned out that way.

## Done!

Whew!  And you should see the Excel (actually LibreOffice) formulas used in the grade book worksheet...
