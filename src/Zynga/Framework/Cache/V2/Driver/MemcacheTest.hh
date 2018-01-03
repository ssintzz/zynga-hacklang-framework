<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Driver\Memcache as MemcacheDriver;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableObject
;

class MemcacheTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    CacheFactory::disableMockDrivers();
    return true;
  }

  public function testGetConfig(): void {

    $obj = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $this->assertTrue($obj->getConfig() instanceof DriverConfigInterface);

  }

  public function testConnect(): void {

    $obj = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    if ($obj instanceof MemcacheDriver) {
      // do a initial connection.
      $this->assertTrue($obj->connect());
      // test that the caching is working
      $this->assertTrue($obj->connect());
    }

  }

  /**
   * @expectedException Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException
   */
  public function testConnect_NoServersConfigured(): void {

    $obj = CacheFactory::factory(
      MemcacheDriver::class,
      'Mock_NoServersConfigured',
    );

    if ($obj instanceof MemcacheDriver) {
      $obj->connect();
    }

  }

  public function testValidCycle(): void {

    $testFloat = 3.14597;
    $testString = 'how now brown cow';
    $testInt = 678912;

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_float->set($testFloat);
    $obj->example_string->set($testString);
    $obj->example_uint64->set($testInt);

    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    // save the object into the cache.
    $this->assertTrue($cache->set($obj));

    // try to pull it back out of the cache
    $back = new ValidStorableObject();
    $back->example_uint64->set($testInt);

    $back = $cache->get($back);

    if ($back instanceof ValidStorableObject) {
      $this->assertEquals($testFloat, $back->example_float->get());
      $this->assertEquals($testString, $back->example_string->get());
    }

    // try to pull it iva the map interface
    $other = $cache->getByMap(Map {'example_uint64' => $testInt});

    if ($other instanceof ValidStorableObject) {
      $this->assertEquals($testInt, $other->example_uint64->get());
      $this->assertEquals($testFloat, $other->example_float->get());
      $this->assertEquals($testString, $other->example_string->get());

      // now remove the thing we just put in.
      $this->assertTrue($cache->delete($other));

      $this->assertEquals(null, $cache->get($other));
      $this->assertEquals(
        null,
        $cache->getByMap(Map {'example_uint64' => $testInt}),
      );

    }

    // now reinject the object back into the cache
    $this->assertTrue($cache->set($obj));

    $again = $cache->get($obj);

    if ($again instanceof ValidStorableObject) {

      // now remove the thing we just put in.
      $this->assertTrue(
        $cache->deleteByMap(Map {'example_uint64' => $testInt}),
      );

      $this->assertEquals(null, $cache->get($again));
      $this->assertEquals(
        null,
        $cache->getByMap(Map {'example_uint64' => $testInt}),
      );

    }

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testGet_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $cache->get($obj);

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testSet_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $cache->set($obj);

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testDelete_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $cache->delete($obj);

  }

  /**
   * @expectedException Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException
   */
  public function testGetByMap_StorableObjectRequired(): void {
    $cache =
      CacheFactory::factory(MemcacheDriver::class, 'Mock_NonStorableObject');
    $cache->getByMap(Map {'foo' => 'bar'});
  }

  /**
   * @expectedException Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException
   */
  public function testDeleteByMap_StorableObjectRequired(): void {
    $cache =
      CacheFactory::factory(MemcacheDriver::class, 'Mock_NonStorableObject');
    $cache->deleteByMap(Map {'foo' => 'bar'});
  }

}
