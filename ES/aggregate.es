GET books/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "match": {
            "author": "Joshua"
          }
        },
        {
          "range": {
            "amazon_rating": {
              "gte": 4.7
            }
          }
        }
      ],
      "must_not": [
        {
          "range": {
            "amazon_rating": {
              "lt": 4.7
            }
          }
        }
      ],
      "should": [
        {
          "match": {
            "tags": "software"
          }
        }
      ],
      "filter": [
        {
          "range": {
            "release_date": {
              "gt": "2015-01-01"
            }
          }
        },
        {
          "term": {
            "edition": 3
          }
        }
      ]
    }
  }
}


POST covid/_bulk
{"index":{}}
{"country":"United States of America","date":"2021-03-26","cases":30853032,"deaths":561142,"recovered":23275268,"critical":8610}
{"index":{}}
{"country":"Brazil","date":"2021-03-26","cases":12407323,"deaths":307326,"recovered":10824095,"critical":8318}
{"index":{}}
{"country":"India","date":"2021-03-26","cases":11908373,"deaths":161275,"recovered":11292849,"critical":8944}
{"index":{}}
{"country":"Russia","date":"2021-03-26","cases":4501859,"deaths":97017,"recovered":4120161,"critical":2300}
{"index":{}}
{"country":"France","date":"2021-03-26","cases":4465956,"deaths":94275,"recovered":288062,"critical":4766}
{"index":{}}
{"country":"United kingdom","date":"2021-03-26","cases":4325315,"deaths":126515,"recovered":3768434,"critical":630}
{"index":{}}
{"country":"Italy","date":"2021-03-26","cases":3488619,"deaths":107256,"recovered":2814652,"critical":3628}
{"index":{}}
{"country":"Spain","date":"2021-03-26","cases":3255324,"deaths":75010,"recovered":3016247,"critical":1830}
{"index":{}}
{"country":"Turkey","date":"2021-03-26","cases":3149094,"deaths":30772,"recovered":2921037,"critical":1810}
{"index":{}}
{"country":"Germany","date":"2021-03-26","cases":2754002,"deaths":76303,"recovered":2467600,"critical":3209}

POST covid/_delete_by_query
{
  "query": {
    "match_all": {}
  }
}

GET covid/_search
GET covid/_count

GET covid/_search
{
  "query": {
    "match": {
      "country": "america"
    }
  }
}

GET covid/_search
{
  "aggs": {
    "critical_patients": {
      "sum": {
        "field": "critical"
      }
    }
  }
}

GET covid/_search
{
  "size": 0,
  "aggs": {
    "critical_patients": {
      "sum": {
        "field": "critical"
      }
    }
  }
}

GET covid/_search
{
  "size": 0,
  "aggs": {
    "max_deaths": {
      "max": {
        "field": "deaths"
      }
    }
  }
}

## all stats (stats, extended_stats)
GET covid/_search
{
  "size": 0,
  "aggs": {
    "all_stat": {
      "extended_stats": {
        "field": "deaths"
      }
    }
  }
}


## histogram
GET covid/_search
{
  "size": 0,
  "aggs": {
    "critical_patient_histogram": {
      "histogram": {
        "field": "critical",
        "interval": 2500
      }
    }
  }
}

## range bucket
GET covid/_search
{
  "size": 0,
  "aggs": {
    "range_countries": {
      "range": {
        "field": "deaths",
        "ranges": [
          {"to": 6000},
          {
            "from": 6000,"to": 7000
          },
          {"from":7000, "to":8000},
          {"from":8000}
        ]
      }
    }
  }
}


























